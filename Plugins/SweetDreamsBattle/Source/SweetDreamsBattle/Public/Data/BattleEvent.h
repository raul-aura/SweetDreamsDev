// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/BattleDataTypes.h"
#include "BattleEvent.generated.h"

class UBattleElement;

UCLASS(Abstract, EditInlineNew, BlueprintType, Blueprintable, DefaultToInstanced)
class SWEETDREAMSBATTLE_API UBattleEvent : public UObject
{
	GENERATED_BODY()

public: 

	void InitializeBattleEvent(UBattleElement* InBattleElement);

	// Called when the Battle Element creates this event. This is called once per Battle Event.
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Event")
	void OnEventCreated();
	virtual void OnEventCreated_Implementation() {}

	// When the Battle Element is executed, all of its events will be executed in a sequential order.
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Event")
	void ExecuteEvent();
	virtual void ExecuteEvent_Implementation() {}

	// If this event is currently being executed by the Battle Element, it will tick as well.
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Event")
	void Tick(float DeltaTime);
	virtual void Tick_Implementation(float DeltaTime) {}

	// Upon returning true, this event will be considered as finished and the Battle Element will progress to execute the next event, if available.
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Event")
	bool IsFinished() const;
	virtual bool IsFinished_Implementation() const;

	// Called when the Battle Element ends. This is useful for cleanup operations.
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Event")
	void EndEvent();
	virtual void EndEvent_Implementation() {}

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Targets", meta = (EditCondition = "bOverrideTargetSettings"))
	FSelectedTargetsSettings TargetSettings;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Targets", meta = (InlineEditConditionToggle))
	bool bOverrideTargetSettings = false;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Battle Event")
	TWeakObjectPtr<UBattleElement> BattleElement;
};

