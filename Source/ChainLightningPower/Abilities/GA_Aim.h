// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Aim.generated.h"

/**
 * 
 */
UCLASS()
class CHAINLIGHTNINGPOWER_API UGA_Aim : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Tags")
	FGameplayTag AimAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Tags")
	FGameplayTag AimAbilityStatusTag;
	
public:
	UGA_Aim();
};
