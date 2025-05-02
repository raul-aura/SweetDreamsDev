// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SweetDreamsSettings.generated.h"

class USweetDreamsSaveFile;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDebugFlags : uint8
{
	NONE = 0 UMETA(Hidden),
	PrintEnabled = 1 << 0,
	PrintSaveOperations = 1 << 1,
};
ENUM_CLASS_FLAGS(EDebugFlags)

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Sweet Dreams Core"))
class SWEETDREAMS_API USweetDreamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public: 
	USweetDreamsSettings();

	// DEBUG
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|Debug", meta = (Bitmask, BitmaskEnum = EDebugFlags))
	int32 DebugFlags;

	// SAVE

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|Settings", meta = (Tooltip = "On Core initialize, the Persistent Save and Local Save will always be created and saved.\n\nSince these functions run on Game Instance Initialize, it cannot call Actor interfaces such as load/save data."))
	bool bEnableAutoCreateSave = true;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|Settings", meta = (Tooltip = "On Core initialize, the Persistent Save file will always be loaded, if any.\n\nSince these functions run on Game Instance Initialize, it cannot call Actor interfaces such as load/save data."))
	bool bEnableAutoLoadSave = true;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|Settings", meta = (Tooltip = "On Game Mode Begin Play, the Persistent Save file will always load data to actors with Sweet Dreams Save Interface, if any.\n\nYour Game Mode needs to be an instance of SweetDreamsGameMode for this to work."))
	bool bEnableAutoLoadData = true;
	
	//Name of the Persistent Save File
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save|Persistent", meta = (DisplayName = "Persistent File Name"))
	FString PersistentSlot = "SweetDream_PERSISTENT";
	//Class of the Persistent Save
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save|Persistent", meta = (DisplayName = "Persistent Save Class"))
	TSubclassOf<USweetDreamsSaveFile> PersistentClass;
	//Name of the Local Save File
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save|Local", meta = (DisplayName = "Local File Name"))
	FString LocalSlot = "SweetDream_LOCAL";
	//Class of the Local Save
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save|Local", meta = (DisplayName = "Local Save Class"))
	TSubclassOf<USweetDreamsSaveFile> LocalClass;
};
