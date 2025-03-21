// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkillMaker : ModuleRules
{
	public SkillMaker(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "SkillMaker" });
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore", "EnhancedInput" });
	}
}
