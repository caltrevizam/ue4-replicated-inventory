// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PathToRagnarok : ModuleRules
{
    public PathToRagnarok(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Slate",
            "NavigationSystem",
            "AIModule",
        });
    }
}
