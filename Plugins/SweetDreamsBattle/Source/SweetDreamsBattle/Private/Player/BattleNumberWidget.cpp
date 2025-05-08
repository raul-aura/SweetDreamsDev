// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/BattleNumberWidget.h"
#include "Player/BattleCharacter.h"

void UBattleNumberWidget::RemoveThisIndicator()
{
	OwningBattler->RemoveDamageIndicator(OwningWidgetComponent);
}
