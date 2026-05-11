// Copyright . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAuraSDKModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

namespace SweetDreams
{
    FORCEINLINE bool IsLoaded()
    {
        return FModuleManager::Get().IsModuleLoaded(TEXT("SweetDreams"));
    }
}

namespace SweetDreamsBattle
{
    FORCEINLINE bool IsLoaded()
    {
        return FModuleManager::Get().IsModuleLoaded(TEXT("SweetDreamsBattle"));
    }
}
