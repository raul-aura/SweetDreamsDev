// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SweetDreamsSettings.generated.h"

class USweetDreamsSaveFile;

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Sweet Dreams Core"))
class SWEETDREAMS_API USweetDreamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public: 
	USweetDreamsSettings();

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game")
	FString GameVersion = TEXT("1.0");

	// LOG
	UPROPERTY(Config, EditAnywhere, Category = "Log")
	bool bLogEnabled = true;
	UPROPERTY(Config, EditAnywhere, Category = "Log|Toggle Specific Logging")
	bool bSaveOperations = true;
	UPROPERTY(Config, EditAnywhere, Category = "Log|Toggle Specific Logging")
	bool bSettingGlobalVariable = true;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Log", meta = (DisplayName = "Info Print Color"))
	FColor InfoColor = FColor(195, 150, 255);
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Log", meta = (DisplayName = "Warning Print Color"))
	FColor WarningColor = FColor(255, 191, 64);
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Log", meta = (DisplayName = "Error Print Color"))
	FColor ErrorColor = FColor(216, 29, 29);

	// SAVE
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (Tooltip = "On Core Initialize(), the Persistent Save and Local Save will always be created and saved."))
	bool bEnableAutoCreateSave = true;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (Tooltip = "On Game Mode StartPlay(), the Persistent and Local save files will always be loaded, if any."))
	bool bEnableAutoLoadSave = true;
	
	//Name of the Persistent Save File
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save", meta = (DisplayName = "Persistent File Name"))
	FString PersistentSlot = "SweetDream_PERSISTENT";
	//Class of the Persistent Save
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save", meta = (DisplayName = "Persistent Save Class"))
	TSubclassOf<USweetDreamsSaveFile> PersistentClass;
};
