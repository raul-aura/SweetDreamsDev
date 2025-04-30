// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleEvent.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew)
class SWEETDREAMSBATTLE_API UBattleEvent : public UObject
{
	GENERATED_BODY()

public: 
	UPROPERTY(EditDefaultsOnly)
	float DamageMultiplier = 1.0f;
};

