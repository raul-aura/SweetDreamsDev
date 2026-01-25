// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/BattleDataTypes.h"
#include "BattleElementData.generated.h"

class UBattleEvent;

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMSBATTLE_API UBattleElementData : public UDataAsset
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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pre-Setup Events")
    FBattleElementEventPhase SetupEvents;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Execution Events")
    FBattleElementEventPhase ExecutionEvents;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "End Events")
    FBattleElementEventPhase EndEvents;
};

