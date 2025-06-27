// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SweetDreamsSaveInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USweetDreamsSaveInterface : public UInterface
{
	GENERATED_BODY()
};

class SWEETDREAMS_API ISweetDreamsSaveInterface
{
	GENERATED_BODY()

public:

	//
	// OnGameSaved, Loaded and etc native events
	//

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	TMap<FName, FString> GetCustomData();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	void LoadCustomData(const TMap<FName, FString>& Data);
};
