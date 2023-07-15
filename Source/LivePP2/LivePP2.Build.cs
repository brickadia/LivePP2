// Copyright Brickadia, LLC. All Rights Reserved.

using UnrealBuildTool;

public class LivePP2 : ModuleRules
{
	public LivePP2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
			new[] {
				"Core",
                "LivePPSDK",
			}
		);			
		
		PrivateDependencyModuleNames.AddRange(
			new[] {
                "Engine",
                "CoreUObject",
                "Projects",
                "DeveloperSettings"
			}
		);
	}
}
