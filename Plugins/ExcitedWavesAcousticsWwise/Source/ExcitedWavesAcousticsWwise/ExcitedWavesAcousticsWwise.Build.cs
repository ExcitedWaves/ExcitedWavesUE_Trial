// Copyright Excited Waves. All Rights Reserved.

using UnrealBuildTool;

public class ExcitedWavesAcousticsWwise : ModuleRules
{
    public ExcitedWavesAcousticsWwise(ReadOnlyTargetRules Target) : base(Target)
    {
		bUsePrecompiled = true;
        PrecompileForTargets = PrecompileTargetsType.Any;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "ExcitedWavesAcoustics",
                "AkAudio"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "DeveloperSettings",
                "WwiseSoundEngine",
                "ExcitedWavesAcousticsSDK"
            }
        );

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "PropertyEditor",
                    "Slate",
                    "SlateCore"
                }
            );
        }
    }
}
