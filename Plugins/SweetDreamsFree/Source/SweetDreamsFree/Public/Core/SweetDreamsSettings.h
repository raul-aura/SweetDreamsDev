// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SweetDreamsSettings.generated.h"

class USweetDreamsSaveFile;

UENUM(BlueprintType)
enum class EDebugFlags : uint8
{
	NONE = 0 UMETA(Hidden),
	PrintEnabled = 1 << 0,
	PrintSaveOperations = 1 << 1,
};
ENUM_CLASS_FLAGS(EDebugFlags)

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Sweet Dreams Core"))
class SWEETDREAMSFREE_API USweetDreamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public: 
	USweetDreamsSettings();

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game")
	FString GameVersion = TEXT("1.0");

	// DEBUG
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (Bitmask, BitmaskEnum = "/Script/SweetDreamsFree.EDebugFlags"))
	int32 DebugFlags;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (DisplayName = "Info Print Color"))
	FColor InfoColor = FColor(195, 150, 255);
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (DisplayName = "Warning Print Color"))
	FColor WarningColor = FColor(255, 191, 64);
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (DisplayName = "Error Print Color"))
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
	//Name of the Local Save File
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save", meta = (DisplayName = "Local File Name"))
	FString LocalSlot = "SweetDream_LOCAL";
	//Class of the Local Save
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save", meta = (DisplayName = "Local Save Class"))
	TSubclassOf<USweetDreamsSaveFile> LocalClass;
};
