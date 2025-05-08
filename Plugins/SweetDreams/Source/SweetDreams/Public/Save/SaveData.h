// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SaveData.generated.h"

USTRUCT(BlueprintType)
struct SWEETDREAMS_API FSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName LevelName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName ActorName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<uint8> ByteData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FName, FString> CustomData;
};
