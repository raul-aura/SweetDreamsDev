// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveData.generated.h"

// A struct that contains data for each actor of all levels active during a save game operation.
USTRUCT(BlueprintType)
struct SWEETDREAMS_API FSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Save Data")
	FName LevelName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Save Data")
	FName ActorName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Save Data")
	TArray<uint8> ByteData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Save Data")
	TMap<FName, FString> CustomData;
};
