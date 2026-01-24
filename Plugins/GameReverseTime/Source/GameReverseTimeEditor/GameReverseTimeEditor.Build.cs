// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using System.Diagnostics;
using System.Collections.Generic;
using UnrealBuildTool;

public class GameReverseTimeEditor : ModuleRules
{
    public GameReverseTimeEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicIncludePaths.Add(Path.Combine(ModulePath, "Public"));
        PublicIncludePaths.Add(Path.Combine(ModulePath, "Public/ExternalEntry"));

        string EnginePath = Path.GetFullPath(Target.RelativeEnginePath);
            PublicIncludePaths.AddRange(new string[] {
            Path.Combine(EnginePath, "Source/Runtime/Engine/Private")
        });

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                // ... add other public dependencies that you statically link with here ...
            }
        );

        PrivateDependencyModuleNames.AddRange(
             new string[]
             {
                "Projects",
                "InputCore",
                "EditorFramework",
                "EditorWidgets",
                "PropertyEditor",
                "UnrealEd",
                "ToolMenus",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "AssetTools",
                "AssetRegistry",
                "Engine",
                "CoreUObject",
                "GameReverseTime",
                "PakFileUtilities"
                 // ... add private dependencies that you statically link with here ...	
             }
             );
    }



    private string ModulePath { get { return ModuleDirectory; } }
}
