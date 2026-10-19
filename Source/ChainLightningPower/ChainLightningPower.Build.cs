// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ChainLightningPower : ModuleRules
{
	public ChainLightningPower(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ChainLightningPower",
			"ChainLightningPower/Variant_Platforming",
			"ChainLightningPower/Variant_Platforming/Animation",
			"ChainLightningPower/Variant_Combat",
			"ChainLightningPower/Variant_Combat/AI",
			"ChainLightningPower/Variant_Combat/Animation",
			"ChainLightningPower/Variant_Combat/Gameplay",
			"ChainLightningPower/Variant_Combat/Interfaces",
			"ChainLightningPower/Variant_Combat/UI",
			"ChainLightningPower/Variant_SideScrolling",
			"ChainLightningPower/Variant_SideScrolling/AI",
			"ChainLightningPower/Variant_SideScrolling/Gameplay",
			"ChainLightningPower/Variant_SideScrolling/Interfaces",
			"ChainLightningPower/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
