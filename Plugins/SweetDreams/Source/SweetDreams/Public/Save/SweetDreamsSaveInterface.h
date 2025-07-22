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

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Save")
	void OnGameSaved(USweetDreamsSaveFile* SaveFile, const FString& Slot);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Save")
	void OnGameLoaded(USweetDreamsSaveFile* SaveFile, const FString& Slot);

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Save")
	TMap<FName, FString> GetCustomData();
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Save")
	void LoadCustomData(const TMap<FName, FString>& Data);
};
