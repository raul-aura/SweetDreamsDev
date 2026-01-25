// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleEvent.generated.h"

UCLASS(Abstract, EditInlineNew, BlueprintType, DefaultToInstanced)
class SWEETDREAMSBATTLE_API UBattleEvent : public UObject
{
	GENERATED_BODY()

public: 
	virtual void Start(/*UBattleContext* Context*/) PURE_VIRTUAL(UBattleEvent::Start, );
	virtual bool IsFinished() const { return true; }

	// damage, heal, add state, remove state, restore mana, remove mana, kill, revive, set ability to act, start animation, start sequence, create particle
	// play sound, trigger another event, start dialogue
};

