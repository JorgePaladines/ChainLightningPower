#include "Abilities/GA_Aim.h"

#include "ChainLightningPowerCharacter.h"
#include "ChainLightningPowerPlayerController.h"

UGA_Aim::UGA_Aim()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	AimAbilityTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Power.Aim"));
	AimAbilityStatusTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Status.IsAiming"));

	AbilityTags.AddTag(AimAbilityTag);
	ActivationOwnedTags.AddTag(AimAbilityStatusTag);

	// Prevent this ability from being activated again while it's already active
	BlockAbilitiesWithTag.AddTag(AimAbilityTag);
}

void UGA_Aim::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (AChainLightningPowerCharacter* Character = Cast<AChainLightningPowerCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->SetAiming(true);
		Character->bUseControllerRotationYaw = true;
	}

	if (AChainLightningPowerPlayerController* PC = Cast<AChainLightningPowerPlayerController>(ActorInfo->PlayerController.Get()))
	{
		PC->ShowAimingUI();
	}
}

void UGA_Aim::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AChainLightningPowerCharacter* Character = Cast<AChainLightningPowerCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->SetAiming(false);
		Character->bUseControllerRotationYaw = false;
	}

	if (AChainLightningPowerPlayerController* PC = Cast<AChainLightningPowerPlayerController>(ActorInfo->PlayerController.Get()))
	{
		PC->HideAimingUI();
	}
}
