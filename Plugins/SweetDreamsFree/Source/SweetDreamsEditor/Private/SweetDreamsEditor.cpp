// Copyright Epic Games, Inc. All Rights Reserved.

#include "SweetDreamsEditor.h"
#include "TSweetDreamsAssetAction.h"
#include "AssetToolsModule.h"
#include "Game/SweetDreamsGameMode.h"
#include "Game/SweetDreamsWidget.h"
#include "Player/SweetDreamsCharacter.h"
#include "Player/SweetDreamsPlayerController.h"
#include "Save/SweetDreamsSaveFile.h"

#define LOCTEXT_NAMESPACE "FSweetDreamsEditorModule"

void FSweetDreamsEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	EAssetTypeCategories::Type SweetDreamsCategory = AssetTools.RegisterAdvancedAssetCategory(FName("SweetDreams"), FText::FromString("Sweet Dreams"));

	AssetTools.RegisterAssetTypeActions(MakeShareable(new SweetDreamsAssetAction(SweetDreamsCategory, ASweetDreamsGameMode::StaticClass())));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new SweetDreamsAssetAction(SweetDreamsCategory, USweetDreamsWidget::StaticClass())));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new SweetDreamsAssetAction(SweetDreamsCategory, ASweetDreamsCharacter::StaticClass())));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new SweetDreamsAssetAction(SweetDreamsCategory, ASweetDreamsPlayerController::StaticClass())));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new SweetDreamsAssetAction(SweetDreamsCategory, USweetDreamsSaveFile::StaticClass())));

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSweetDreamsEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSweetDreamsEditorModule, SweetDreamsEditor)