// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ChainLightningAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class CHAINLIGHTNINGPOWER_API UChainLightningAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UChainLightningAttributeSet();
	
	/** Theorical damage for the enemy */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS_BASIC(UChainLightningAttributeSet, Damage);
	
	/** Radius of the hit detection */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_BounceRadius)
	FGameplayAttributeData BounceRadius;
	ATTRIBUTE_ACCESSORS_BASIC(UChainLightningAttributeSet, BounceRadius);
	
	/** Max amount of bounces that the chain can perform */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_MaxBounces)
	FGameplayAttributeData MaxBounces;
	ATTRIBUTE_ACCESSORS_BASIC(UChainLightningAttributeSet, MaxBounces);
	
	/** Delay of the bounce between each Chain Level */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_BounceDelay)
	FGameplayAttributeData BounceDelay;
	ATTRIBUTE_ACCESSORS_BASIC(UChainLightningAttributeSet, BounceDelay);
	
	/** Delay before the chain connection is executed */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_ChainLinkDelay)
	FGameplayAttributeData ChainLinkDelay;
	ATTRIBUTE_ACCESSORS_BASIC(UChainLightningAttributeSet, ChainLinkDelay);
	
	/** Duration length of the visual chain effect */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_EffectDuration)
	FGameplayAttributeData EffectDuration;
	ATTRIBUTE_ACCESSORS_BASIC(UChainLightningAttributeSet, EffectDuration);
	
	/** Duration length of the enemy stun or electrocution */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_StunDuration)
	FGameplayAttributeData StunDuration;
	ATTRIBUTE_ACCESSORS_BASIC(UChainLightningAttributeSet, StunDuration);
	
	/** Max allowed number of lightning chains that a single source enemy can make to other enemies */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_ChainAmount)
	FGameplayAttributeData ChainAmount;
	ATTRIBUTE_ACCESSORS_BASIC(UChainLightningAttributeSet, ChainAmount);
	
	/** Ability Cooldown duration */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_Cooldown)
	FGameplayAttributeData Cooldown;
	ATTRIBUTE_ACCESSORS_BASIC(UChainLightningAttributeSet, Cooldown);
	
public:
	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UChainLightningAttributeSet, Damage, OldValue);
	}
	
	UFUNCTION()
	void OnRep_BounceRadius(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UChainLightningAttributeSet, BounceRadius, OldValue);
	}
	
	UFUNCTION()
	void OnRep_MaxBounces(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UChainLightningAttributeSet, MaxBounces, OldValue);
	}
	
	UFUNCTION()
	void OnRep_BounceDelay(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UChainLightningAttributeSet, BounceDelay, OldValue);
	}
	
	UFUNCTION()
	void OnRep_ChainLinkDelay(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UChainLightningAttributeSet, ChainLinkDelay, OldValue);
	}
	
	UFUNCTION()
	void OnRep_EffectDuration(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UChainLightningAttributeSet, EffectDuration, OldValue);
	}
	
	UFUNCTION()
	void OnRep_StunDuration(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UChainLightningAttributeSet, StunDuration, OldValue);
	}
	
	UFUNCTION()
	void OnRep_ChainAmount(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UChainLightningAttributeSet, ChainAmount, OldValue);
	}
	
	UFUNCTION()
    void OnRep_Cooldown(const FGameplayAttributeData& OldValue) const
    {
    	GAMEPLAYATTRIBUTE_REPNOTIFY(UChainLightningAttributeSet, Cooldown, OldValue);
    }
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
