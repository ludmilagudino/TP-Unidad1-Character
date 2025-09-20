// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPUnidad1Gudino : ModuleRules
{
	public TPUnidad1Gudino(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TPUnidad1Gudino",
			"TPUnidad1Gudino/Variant_Platforming",
			"TPUnidad1Gudino/Variant_Platforming/Animation",
			"TPUnidad1Gudino/Variant_Combat",
			"TPUnidad1Gudino/Variant_Combat/AI",
			"TPUnidad1Gudino/Variant_Combat/Animation",
			"TPUnidad1Gudino/Variant_Combat/Gameplay",
			"TPUnidad1Gudino/Variant_Combat/Interfaces",
			"TPUnidad1Gudino/Variant_Combat/UI",
			"TPUnidad1Gudino/Variant_SideScrolling",
			"TPUnidad1Gudino/Variant_SideScrolling/AI",
			"TPUnidad1Gudino/Variant_SideScrolling/Gameplay",
			"TPUnidad1Gudino/Variant_SideScrolling/Interfaces",
			"TPUnidad1Gudino/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
