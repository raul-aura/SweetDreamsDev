// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SweetDreamsSaveFile.h"
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	TMap<FName, FString> GetPersistentCustomData();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	TMap<FName, FString> GetPersistentDeltaData();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	void LoadPersistentCustomData(const TMap<FName, FString>& Data);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	void LoadPersistentDeltaData(const TMap<FName, FString>& Data);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	TMap<FName, FString> GetLocalCustomData();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	TMap<FName, FString> GetLocalDeltaData();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	void LoadLocalCustomData(const TMap<FName, FString>& Data);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	void LoadLocalDeltaData(const TMap<FName, FString>& Data);

	UFUNCTION(BlueprintNativeEvent)
	bool IsDeltaSaveEnabled();
};
