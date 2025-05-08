// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BattleElementData.generated.h"

class UBattleEvent;

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMSBATTLE_API UBattleElement : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Element")
    FName ElementUniqueName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Element")
    FText ElementName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Element", meta = (MultiLine = true))
    FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Element", meta = (MultiLine = true))
    FText Message;

    UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "Pre-Setup Events")
    TArray<UBattleEvent*> PreSetupEvents;

    UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "Setup Events")
    TArray<UBattleEvent*> SetupEvents;

    UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "Execution Events")
    TArray<UBattleEvent*> ExecutionEvents;

    UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "End Events")
    TArray<UBattleEvent*> EndEvents;
};

