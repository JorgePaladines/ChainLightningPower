// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChainLightningPowerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ChainLightningPower.h"
#include "AttributeSets/ChainLightningAttributeSet.h"
#include "Net/UnrealNetwork.h"

AChainLightningPowerCharacter::AChainLightningPowerCharacter()
{
	bReplicates = true;
	
	// Add the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(AscReplicationMode);
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DefaultArmLength;
	CameraBoom->SocketOffset = DefaultSocketOffset;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AChainLightningPowerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PC = Cast<APlayerController>(Controller); PC && PC->PlayerCameraManager)
	{
		PC->PlayerCameraManager->ViewPitchMin = -60.0f;
		PC->PlayerCameraManager->ViewPitchMax = 70.0f;
	}
}

void AChainLightningPowerCharacter::OnRep_IsAiming(const bool OldValue)
{
	// 
}

void AChainLightningPowerCharacter::OnRep_Pitch(const float OldValue)
{
	//
}

void AChainLightningPowerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AChainLightningPowerCharacter, bIsAiming, COND_SkipOwner, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(AChainLightningPowerCharacter, PitchInput, COND_SkipOwner, REPNOTIFY_Always);
}

void AChainLightningPowerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// The set is spawned by the ASC from DefaultStartingData; cache that instance instead of creating a second one
	if (AbilitySystemComponent)
	{
		ChainLightningAttributeSet = const_cast<UChainLightningAttributeSet*>(AbilitySystemComponent->GetSet<UChainLightningAttributeSet>());
	}
}

void AChainLightningPowerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// The camera only matters to the player who owns it
	if (!IsLocallyControlled() || !CameraBoom)
	{
		return;
	}

	const float TargetArmLength = bIsAiming ? AimArmLength : DefaultArmLength;
	const FVector TargetSocketOffset = bIsAiming ? AimSocketOffset : DefaultSocketOffset;

	if (AimInterpSpeed <= 0.0f)
	{
		CameraBoom->TargetArmLength = TargetArmLength;
		CameraBoom->SocketOffset = TargetSocketOffset;
		return;
	}

	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArmLength, DeltaSeconds, AimInterpSpeed);
	CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetSocketOffset, DeltaSeconds, AimInterpSpeed);
}

void AChainLightningPowerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Initialize the Ability System Component for the client
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AChainLightningPowerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		for (const auto& EachAbility : InitialAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(EachAbility));
		}
	}
}

UAbilitySystemComponent* AChainLightningPowerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AChainLightningPowerCharacter::CancelAbilitiesWithTags(const FGameplayTagContainer& WithTags)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities(&WithTags);
	}
}

void AChainLightningPowerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AChainLightningPowerCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AChainLightningPowerCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AChainLightningPowerCharacter::Look);
		
		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AChainLightningPowerCharacter::Fire);
		
		// Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AChainLightningPowerCharacter::AimStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AChainLightningPowerCharacter::AimEnd);
	}
	else
	{
		UE_LOG(LogChainLightningPower, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AChainLightningPowerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AChainLightningPowerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AChainLightningPowerCharacter::Fire(const FInputActionValue& Value)
{
	DoFire();
}

void AChainLightningPowerCharacter::AimStart(const FInputActionValue& Value)
{
	DoAimStart();
}

void AChainLightningPowerCharacter::AimEnd(const FInputActionValue& Value)
{
	DoAimEnd();
}

void AChainLightningPowerCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AChainLightningPowerCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
		
		const float CurrentPitch = FRotator::NormalizeAxis(GetControlRotation().Pitch);
		if (!FMath::IsNearlyEqual(CurrentPitch, PitchInput, 0.5f))
		{
			PitchInput = CurrentPitch;
			if (!HasAuthority())
			{
				ServerSetPitch(CurrentPitch);
			}
		}
	}
}

void AChainLightningPowerCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AChainLightningPowerCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AChainLightningPowerCharacter::DoAimStart()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	FGameplayTagContainer Tag;
	Tag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Abilities.Power.Aim")));
	AbilitySystemComponent->TryActivateAbilitiesByTag(Tag);
}

void AChainLightningPowerCharacter::DoAimEnd()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	FGameplayTagContainer Tag;
	Tag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Abilities.Power.Aim")));
	AbilitySystemComponent->CancelAbilities(&Tag);
}

void AChainLightningPowerCharacter::SetAiming(bool bAim)
{
	bIsAiming = bAim;
	if (!HasAuthority())
	{
		ServerSetIsAiming(bAim);
	}
}

void AChainLightningPowerCharacter::ServerSetIsAiming_Implementation(bool bAim)
{
	SetAiming(bAim);
}

void AChainLightningPowerCharacter::ServerSetPitch_Implementation(float NewPitch)
{
	PitchInput = NewPitch;
}

float AChainLightningPowerCharacter::GetRawPitchForAnim() const
{
	if (IsLocallyControlled())
	{
		return -FRotator::NormalizeAxis(GetControlRotation().Pitch);
	}
	return -PitchInput;
}