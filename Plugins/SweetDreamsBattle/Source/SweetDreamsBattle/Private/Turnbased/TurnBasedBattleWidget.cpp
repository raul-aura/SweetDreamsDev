// Fill out your copyright notice in the Description page of Project Settings.


#include "Turnbased/TurnBasedBattleWidget.h"
#include "Turnbased/TurnBasedBattle.h"

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
