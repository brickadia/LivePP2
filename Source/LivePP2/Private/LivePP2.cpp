// Copyright Brickadia, LLC. All Rights Reserved.

#include "LivePP2.h"
#include "Interfaces/IPluginManager.h"

THIRD_PARTY_INCLUDES_START
#include "Windows/AllowWindowsPlatformTypes.h"
#include "LPP_API_x64_CPP.h"
#include "Windows/HideWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_END

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

	GSynchronizedAgent = lpp::LppCreateSynchronizedAgent(*AgentPath);
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
			if (GSynchronizedAgent.WantsReload())
			{
				GSynchronizedAgent.CompileAndReloadChanges(lpp::LPP_RELOAD_BEHAVIOUR_WAIT_UNTIL_CHANGES_ARE_APPLIED);
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
