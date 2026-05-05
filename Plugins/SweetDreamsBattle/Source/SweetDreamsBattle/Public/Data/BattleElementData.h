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

    /* If true, the element will automatically end when all its events finish.
    If false, the element remains active and must be ended manually with End(). */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Element")
    bool bAutoEndOnEventsComplete = true;

    UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "Events")
    TArray<TObjectPtr<UBattleEvent>> Events;
};

