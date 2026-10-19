// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ChainLightningPowerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AChainLightningPowerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AbilitySystem")
	EGameplayEffectReplicationMode AscReplicationMode = EGameplayEffectReplicationMode::Mixed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> InitialAbilities;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	
	/** Fire Chain Lightning Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AimAction;
	
	/** Camera boom arm length while not aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera|Default")
	float DefaultArmLength = 300.0f;

	/** Camera boom socket offset while not aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera|Default")
	FVector DefaultSocketOffset = FVector(0.0f, 0.0f, 70.0f);

	/** Camera boom arm length while aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera|Aim")
	float AimArmLength = 150.0f;

	/** Camera boom socket offset while aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera|Aim")
	FVector AimSocketOffset = FVector(0.0f, 50.0f, 70.0f);

	/** How fast the camera moves between the default and aim positions. Higher = snappier, 0 = instant */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera|Aim", meta=(ClampMin="0.0"))
	float AimInterpSpeed = 10.0f;

	UPROPERTY(ReplicatedUsing = OnRep_IsAiming)
	bool bIsAiming;
	
	UPROPERTY(ReplicatedUsing = OnRep_Pitch)
	float PitchInput = 0.0f;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	/** Called for Fire input */
	void Fire(const FInputActionValue& Value);
	
	void AimStart(const FInputActionValue& Value);
	
	void AimEnd(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnRep_IsAiming(const bool OldValue);
	
	UFUNCTION(Server, Reliable)
	void ServerSetIsAiming(bool bAim);
	
	UFUNCTION(Server, Unreliable)
	void ServerSetPitch(float NewPitch);

public:
	/** Constructor */
	AChainLightningPowerCharacter();
	
	virtual void NotifyControllerChanged() override;
	
	/** Ability System Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "AbilitySystem")
	class UChainLightningAttributeSet* ChainLightningAttributeSet;
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetAiming(bool bAim);
	
	UFUNCTION()
	void OnRep_Pitch(const float OldValue);
	
	UFUNCTION(BlueprintPure, Category="Input")
	float GetRawPitchForAnim() const;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void CancelAbilitiesWithTags(const FGameplayTagContainer& WithTags);
	
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void OnRep_PlayerState() override;

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void PossessedBy(AController* NewController) override;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	
	/** Handles fire inputs */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Input")
	void DoFire();
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoAimStart();
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoAimEnd();
};

