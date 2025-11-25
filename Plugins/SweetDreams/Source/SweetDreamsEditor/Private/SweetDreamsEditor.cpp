// Copyright Epic Games, Inc. All Rights Reserved.

#include "SweetDreamsEditor.h"
#include "SweetDreamsAssetActions.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FSweetDreamsEditorModule"

void FSweetDreamsEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	EAssetTypeCategories::Type SweetDreamsCategory = AssetTools.RegisterAdvancedAssetCategory(FName("SweetDreams"), FText::FromString("Sweet Dreams"));

	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_SweetDreamsGameMode(SweetDreamsCategory)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_SweetDreamsCharacter(SweetDreamsCategory)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_SweetDreamsPlayerController(SweetDreamsCategory)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_SweetDreamsWidget(SweetDreamsCategory)));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_SweetDreamsSaveFile(SweetDreamsCategory)));

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSweetDreamsEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSweetDreamsEditorModule, SweetDreamsEditor)