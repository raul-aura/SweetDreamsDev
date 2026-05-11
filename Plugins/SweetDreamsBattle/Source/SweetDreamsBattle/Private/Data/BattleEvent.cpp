// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/BattleEvent.h"
#include "Data/BattleElement.h"

void UBattleEvent::InitializeBattleEvent(UBattleElement* InBattleElement)
{
	BattleElement = InBattleElement;
}

bool UBattleEvent::IsFinished_Implementation() const
{
	return true;
}
