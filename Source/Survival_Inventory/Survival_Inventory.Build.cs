// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Survival_Inventory : ModuleRules
{
	public Survival_Inventory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
