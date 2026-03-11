using UnrealBuildTool;
using System;
using System.IO;

public class ExcitedWavesAcousticsSDK : ModuleRules
{
    public ExcitedWavesAcousticsSDK(ReadOnlyTargetRules Target) : base(Target)
    {
		bUsePrecompiled = true;
        PrecompileForTargets = PrecompileTargetsType.Any;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
        );

        SharedPCHHeaderFile = "Public/ExcitedWavesAcousticsSDK.h";

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string pluginDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
            string binariesDir = Path.Combine(pluginDir, "Binaries", "Win64");

            string configName = Target.Configuration.ToString();

            string[] searchConfigs = new[] { configName, "Development", "Release", "Shipping", "DebugGame", "Debug" };

            string resolvedLibPath = null;
            string resolvedDllPath = null;

            foreach (string cfg in searchConfigs)
            {
                string libCandidate = Path.Combine(binariesDir, cfg, "ExcitedWavesAcousticsSDK.lib");
                if (File.Exists(libCandidate))
                {
                    resolvedLibPath = libCandidate;
                    string dllCandidate = Path.Combine(binariesDir, cfg, "ExcitedWavesAcousticsSDK.dll");
                    if (File.Exists(dllCandidate))
                    {
                        resolvedDllPath = dllCandidate;
                    }
                    break;
                }
            }

            // Fallback: check binariesDir root
            if (resolvedLibPath == null)
            {
                string rootLib = Path.Combine(binariesDir, "ExcitedWavesAcousticsSDK.lib");
                if (File.Exists(rootLib))
                {
                    resolvedLibPath = rootLib;
                    string rootDll = Path.Combine(binariesDir, "ExcitedWavesAcousticsSDK.dll");
                    if (File.Exists(rootDll))
                    {
                        resolvedDllPath = rootDll;
                    }
                }
            }

            bool bIsStatic = (resolvedLibPath != null && resolvedDllPath == null);

            if (resolvedLibPath != null)
            {
                PublicAdditionalLibraries.Add(resolvedLibPath);
                Console.WriteLine($"ExcitedWavesAcousticsSDK: Using library: {resolvedLibPath}");
            }
            else
            {
                Console.WriteLine("ExcitedWavesAcousticsSDK: ERROR - .lib not found in any config folder!");
            }

            if (bIsStatic)
            {
                PublicDefinitions.Add("EWSDK_STATIC=1");
                Console.WriteLine("ExcitedWavesAcousticsSDK: Static linking mode (no DLL)");
            }
            else
            {
                if (resolvedDllPath != null)
                {
                    PublicDelayLoadDLLs.Add("ExcitedWavesAcousticsSDK.dll");
                    RuntimeDependencies.Add(resolvedDllPath, StagedFileType.NonUFS);
                    Console.WriteLine($"ExcitedWavesAcousticsSDK: Dynamic linking mode, DLL: {resolvedDllPath}");
                }
                else
                {
                    Console.WriteLine("ExcitedWavesAcousticsSDK: WARNING - .lib found but no .dll, and not detected as static. Check your build.");
                }
            }
        }
        else
        {
            Console.WriteLine("ExcitedWavesAcousticsSDK: platform not supported for external SDK.");
        }
    }
}
