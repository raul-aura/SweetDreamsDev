// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/BattleWidget.h"

void UBattleWidget::InitializeAndIndicateEffect(float Value, FGameplayTagContainer Tags, bool bIsDamage)
{
	EffectValue = Value;
	EffectTags = Tags;

	if (bIsDamage)
	{
		IndicateDamage(EffectValue);
	}
	else
	{
		IndicateHeal(EffectValue);
	}
}
