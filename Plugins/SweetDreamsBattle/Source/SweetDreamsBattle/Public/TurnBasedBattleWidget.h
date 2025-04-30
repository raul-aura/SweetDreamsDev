// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/SweetDreamsWidget.h"
#include "TurnBasedBattleWidget.generated.h"

class ATurnBasedBattle;

UCLASS()
class SWEETDREAMSBATTLE_API UTurnBasedBattleWidget : public USweetDreamsWidget
{
	GENERATED_BODY()
	
public:
	UTurnBasedBattleWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnBattleStarted();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnBattleEnded(bool bIsVictorious = true);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnElementMessage(const FText& Message);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowPlayerInput(UBattleInputAction* InputAction);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HidePlayerInput();

	UFUNCTION(BlueprintCallable)
	void SetBattleManager(ATurnBasedBattle* Battle);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams RPG")
	ATurnBasedBattle* BattleManager;
};
