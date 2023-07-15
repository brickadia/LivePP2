// Copyright Brickadia, LLC. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System.Linq;

public class LivePPSDK : ModuleRules
{
    public LivePPSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string SdkPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "LivePP"));
            PublicSystemIncludePaths.Add(Path.Combine(SdkPath, "API"));
            
            Directory.GetFiles(Path.Combine(SdkPath, "Agent", "x64"), "*", SearchOption.AllDirectories).ToList().ForEach(File => RuntimeDependencies.Add(File));
            Directory.GetFiles(Path.Combine(SdkPath, "Broker"), "*", SearchOption.AllDirectories).ToList().ForEach(File => RuntimeDependencies.Add(File));
		}
	}
}
