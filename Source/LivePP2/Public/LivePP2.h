// Copyright Brickadia, LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class LIVEPP2_API FLivePP2Module : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    FDelegateHandle OnEndFrameHandle;
};
