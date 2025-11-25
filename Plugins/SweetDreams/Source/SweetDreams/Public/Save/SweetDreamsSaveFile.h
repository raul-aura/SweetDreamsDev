// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save/SaveData.h"
#include "SweetDreamsSaveFile.generated.h"

UCLASS()
class SWEETDREAMS_API USweetDreamsSaveFile : public USaveGame
{
	GENERATED_BODY()

public:
	USweetDreamsSaveFile();

	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams|Core|Save")
	TArray<FSaveData> SavedData;
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams|Core|Save")
	FDateTime LastTimeSaved;
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams|Core|Save")
	int32 AmountOfSaves = 0;
};
