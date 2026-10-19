#include "AttributeSets/ChainLightningAttributeSet.h"
#include "Net/UnrealNetwork.h"

UChainLightningAttributeSet::UChainLightningAttributeSet()
{
	
}

void UChainLightningAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UChainLightningAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UChainLightningAttributeSet, MaxBounces, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UChainLightningAttributeSet, BounceRadius, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UChainLightningAttributeSet, BounceDelay, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UChainLightningAttributeSet, ChainLinkDelay, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UChainLightningAttributeSet, EffectDuration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UChainLightningAttributeSet, StunDuration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UChainLightningAttributeSet, ChainAmount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UChainLightningAttributeSet, Cooldown, COND_None, REPNOTIFY_Always);
}