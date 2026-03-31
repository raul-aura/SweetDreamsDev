// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/BattleDataTypes.h"
#include "BattleParameterEvent.generated.h"

UCLASS(Abstract, EditInlineNew, BlueprintType, Blueprintable, DefaultToInstanced)
class SWEETDREAMSBATTLE_API UBattleParameterEvent : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Parameter Event")
	void OnModifierAdded(const FBattleParameterModifier& Modifier);
	void OnModifierAdded_Implementation(const FBattleParameterModifier& Modifier) {}

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Parameter Event")
	void OnModifierRemoved(const FBattleParameterModifier& Modifier);
	void OnModifierRemoved_Implementation(const FBattleParameterModifier& Modifier) {}

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Parameter Event")
	void OnResourceIncreased(float NewValue, float PreviousValue);
	void OnResourceIncreased_Implementation(float NewValue, float PreviousValue) {}

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Parameter Event")
	void OnResourceDecreased(float NewValue, float PreviousValue);
	void OnResourceDecreased_Implementation(float NewValue, float PreviousValue) {}
};
