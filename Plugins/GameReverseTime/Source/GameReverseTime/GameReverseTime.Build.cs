// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using System.Diagnostics;
using System.Collections.Generic;
using UnrealBuildTool;

public class GameReverseTime : ModuleRules
{
    public GameReverseTime(ReadOnlyTargetRules Target) : base(Target)
    {   
        PublicIncludePaths.Add(Path.Combine(ModulePath, "Public"));
        PublicIncludePaths.Add(Path.Combine(ModulePath, "Public/Core"));

        PublicDependencyModuleNames.AddRange(new string[]
        {
             "Core"
            ,"CoreUObject"
            ,"Engine"
            ,"RenderCore"
            ,"RHI"
            ,"UMG"
            ,"Slate"
            ,"Paper2D"
            ,"SlateCore"
            ,"Projects"
            ,"JsonBlueprintUtilities"
        });
    }

    private string ModulePath { get { return ModuleDirectory; } }
}
