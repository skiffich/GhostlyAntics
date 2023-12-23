// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GhostlyAntics : ModuleRules
{
	public GhostlyAntics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTasks", "UMG" });

		// AI Dependencies
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "NavigationSystem" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
