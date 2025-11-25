// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SweetDreamsSaveInterface.generated.h"

class USweetDreamsSaveFile;

UINTERFACE(MinimalAPI, Blueprintable)
class USweetDreamsSaveInterface : public UInterface
{
	GENERATED_BODY()
};

class SWEETDREAMS_API ISweetDreamsSaveInterface
{
	GENERATED_BODY()

public:

	// Implementable event for when the SweetDreamsCore subsystem saves a file.
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Save")
	void OnGameSaved(USweetDreamsSaveFile* SaveFile, const FString& Slot);
	// Implementable event for when the SweetDreamsCore subsytem loads a save file.
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Save")
	void OnGameLoaded(USweetDreamsSaveFile* SaveFile, const FString& Slot);

	// Implementable event to get custom data as a string and save it to the save file.
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Save")
	TMap<FName, FString> GetCustomData();
	// Implementable event that returns custom data as a string when a save file is loaded.
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Save")
	void LoadCustomData(const TMap<FName, FString>& Data);
};
