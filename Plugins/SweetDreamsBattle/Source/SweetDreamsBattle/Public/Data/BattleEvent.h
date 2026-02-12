// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleEvent.generated.h"

class UBattleContext;

UCLASS(Abstract, EditInlineNew, BlueprintType, DefaultToInstanced)
class SWEETDREAMSBATTLE_API UBattleEvent : public UObject
{
	GENERATED_BODY()

public: 
	virtual void Start() PURE_VIRTUAL(UBattleEvent::Start, );
	virtual void Update(float DeltaTime) {}
	virtual bool IsFinished() const { return true; }
	virtual void Reset() {}

	UPROPERTY(BlueprintReadOnly, Category = "Event")
	TObjectPtr<UBattleContext> BattleContext = nullptr;
};

