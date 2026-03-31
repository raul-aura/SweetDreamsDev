// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "BattleWidget.generated.h"

class UCanvasPanelSlot;

UCLASS()
class SWEETDREAMSBATTLE_API UBattleWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "SweetDreams|Battle|Widget")
	void InitializeAndIndicateEffect(float Value, FGameplayTagContainer Tags, bool bIsDamage = true);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SweetDreams|Battle|Widget")
	void IndicateDamage(float Damage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SweetDreams|Battle|Widget")
	void IndicateHeal(float Heal);

	// Useful for pooling of widgets
	bool bIsActive = false;
	// Useful for pooling of widgets
	UCanvasPanelSlot* CachedSlot;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Battle Effect")
	float EffectValue = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Battle Effect")
	FGameplayTagContainer EffectTags;

};

