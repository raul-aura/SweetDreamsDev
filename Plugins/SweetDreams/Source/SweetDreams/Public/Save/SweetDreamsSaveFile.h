// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save/SaveData.h"
#include "SweetDreamsSaveFile.generated.h"

UCLASS(Abstract, NotBlueprintable)
class SWEETDREAMS_API USweetDreamsSaveFile : public USaveGame
{
	GENERATED_BODY()

public:
	USweetDreamsSaveFile();

	void UpdateCurrentVersion();

	UPROPERTY()
	TArray<FSaveData> SavedFullData;
	UPROPERTY()
	TArray<FDeltaSaveData> SavedDeltaData;
	UPROPERTY()
	int32 CurrentVersion = 0;

};
