// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleInputAction.generated.h"

class UTurnBasedBattleWidget;

UCLASS()
class SWEETDREAMSBATTLE_API UBattleInputAction : public UBattleElement
{
	GENERATED_BODY()
	
public:
	UBattleInputAction();

protected:
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* InputWidget;
};
