// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedBattleWidget.h"
#include "TurnBasedBattle.h"

UTurnBasedBattleWidget::UTurnBasedBattleWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WidgetName = "TurnbasedBattle";
}

void UTurnBasedBattleWidget::SetBattleManager(ATurnBasedBattle* Battle)
{
	if (!IsValid(Battle))
	{
		return;
	}
	BattleManager = Battle;
}
