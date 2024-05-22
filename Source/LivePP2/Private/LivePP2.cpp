// Copyright Brickadia, LLC. All Rights Reserved.

#include "LivePP2.h"

#include "HAL/PlatformProcess.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/CommandLine.h"
#include "Misc/CoreDelegates.h"
#include "Misc/Paths.h"

THIRD_PARTY_INCLUDES_START
#include "Windows/AllowWindowsPlatformTypes.h"
#include "LPP_API_x64_CPP.h"
#include "Windows/HideWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_END

lpp::LppLocalPreferences GLocalPreferences;
lpp::LppProjectPreferences GAgentPreferences;
lpp::LppSynchronizedAgent GSynchronizedAgent;

void FLivePP2Module::StartupModule()
{
	if (!FParse::Param(FCommandLine::Get(), TEXT("LivePP2")))
	{
		return;
	}

	// Initialize agent.
	FString BaseDir = IPluginManager::Get().FindPlugin("LivePP2")->GetBaseDir();
	FString AgentPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/LivePPSDK/LivePP"));

	GLocalPreferences = lpp::LppCreateDefaultLocalPreferences();

	GAgentPreferences = lpp::LppCreateDefaultProjectPreferences();
	GAgentPreferences.hotReload.sourcePathFilters = ".gen.cpp";
	GAgentPreferences.compiler.removeShowIncludes = true;
	GAgentPreferences.compiler.removeSourceDependencies = true;
	GAgentPreferences.exceptionHandler.isEnabled = false;

	GSynchronizedAgent = LppCreateSynchronizedAgentWithPreferences(&GLocalPreferences, *AgentPath, &GAgentPreferences);
	check(lpp::LppIsValidSynchronizedAgent(&GSynchronizedAgent));

	if constexpr (WITH_EDITOR)
	{
		// Enable all prior modules.
		TArray<FModuleStatus> ModuleStatuses;
		TArray<const TCHAR*> ModuleNames;

		FModuleManager::Get().QueryModules(ModuleStatuses);
		ModuleNames.Reserve(ModuleStatuses.Num());

		for (const FModuleStatus& ModuleStatus : ModuleStatuses)
		{
			if (ModuleStatus.bIsLoaded)
			{
				ModuleNames.Add(*ModuleStatus.FilePath);
			}
		}
	
		GSynchronizedAgent.EnableModules(ModuleNames.GetData(), ModuleNames.Num(), lpp::LPP_MODULES_OPTION_ALL_IMPORT_MODULES, nullptr, nullptr);

		// Load all future modules.
		GSynchronizedAgent.EnableAutomaticHandlingOfDynamicallyLoadedModules(nullptr, nullptr);
	}
	else
	{
		// Load the executable itself.
		GSynchronizedAgent.EnableModule(FPlatformProcess::ExecutablePath(), lpp::LPP_MODULES_OPTION_ALL_IMPORT_MODULES, nullptr, nullptr);
	}

	// Bind reload delegate.
	OnEndFrameHandle = FCoreDelegates::OnEndFrame.AddLambda(
		[]{
			if (GSynchronizedAgent.WantsReload(lpp::LPP_RELOAD_OPTION_SYNCHRONIZE_WITH_RELOAD))
			{
				GSynchronizedAgent.Reload(lpp::LPP_RELOAD_BEHAVIOUR_WAIT_UNTIL_CHANGES_ARE_APPLIED);
			}

			if (GSynchronizedAgent.WantsRestart())
			{
				// The others don't seem to work. We would need to advance a frame to handle graceful termination,
				// but this function doesn't return until the process is terminated.
				GSynchronizedAgent.Restart(lpp::LPP_RESTART_BEHAVIOUR_INSTANT_TERMINATION, 0);
			}
		}
	);
}

void FLivePP2Module::ShutdownModule()
{
	if (OnEndFrameHandle.IsValid())
	{
		FCoreDelegates::OnEndFrame.Remove(OnEndFrameHandle);
	}

	LppDestroySynchronizedAgent(&GSynchronizedAgent);
}

IMPLEMENT_MODULE(FLivePP2Module, LivePP2)
