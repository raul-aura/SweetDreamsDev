// Fill out your copyright notice in the Description page of Project Settings.


#include "Turnbased/BattleInputAction.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Turnbased/TurnBasedBattle.h"
#include "Turnbased/TurnBasedBattleWidget.h"

UBattleInputAction::UBattleInputAction()
{
	ElementName = FText::FromString(TEXT("Input"));
	bOverrideOwnerSpeed = true;
	ActionSpeed = 999;
	Cost = 0;
}

void UBattleInputAction::StartAction(bool bUseCooldown)
{
	if (!LoadWidget()) return;
	Super::StartAction(false);
}

bool UBattleInputAction::LoadWidget()
{
	ATurnBasedBattle* TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
	if (!IsValid(TurnBattle))
	{
		int32 Temp = 0;
		TurnBattle = ATurnBasedBattle::FindActiveTurnBattle(GetOwner(), Temp);
	}
	if (IsValid(TurnBattle))
	{
		InputWidget = TurnBattle->GetTurnBattleWidget();
	}
	return InputWidget != nullptr;
}

bool UBattleInputAction::UpdateValidTargets()
{
	return IsValid(GetOwner());
}
