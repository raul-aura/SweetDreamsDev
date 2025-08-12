// Copyright Epic Games, Inc. All Rights Reserved.

#include "DreamScapeGenerator.h"
#include "DreamScapeGeneratorEdMode.h"

#define LOCTEXT_NAMESPACE "FDreamScapeGeneratorModule"

void FDreamScapeGeneratorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FDreamScapeGeneratorEdMode>(FDreamScapeGeneratorEdMode::EM_DreamScapeGeneratorEdModeId, LOCTEXT("DreamScapeGeneratorEdModeName", "DreamScapeGeneratorEdMode"), FSlateIcon(), true);
}

void FDreamScapeGeneratorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FDreamScapeGeneratorEdMode::EM_DreamScapeGeneratorEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDreamScapeGeneratorModule, DreamScapeGenerator)