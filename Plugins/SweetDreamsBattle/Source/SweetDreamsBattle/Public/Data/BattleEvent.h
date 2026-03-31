// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/BattleDataTypes.h"
#include "Data/BattleContext.h"
#include "BattleEvent.generated.h"

class UBattleContext;

UCLASS(Abstract, EditInlineNew, BlueprintType, Blueprintable, DefaultToInstanced)
class SWEETDREAMSBATTLE_API UBattleEvent : public UObject
{
	GENERATED_BODY()

public: 

	// Called when the Battle Element creates this event. This is called once per Battle Element.
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Event")
	void OnEventCreated();
	virtual void OnEventCreated_Implementation() {}
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Event")
	void ExecuteEvent();
	virtual void ExecuteEvent_Implementation() {}
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Event")
	void Tick(float DeltaTime);
	virtual void Tick_Implementation(float DeltaTime) {}
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Event")
	bool IsFinished() const;
	virtual bool IsFinished_Implementation() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Event")
	void EndEvent();
	virtual void EndEvent_Implementation() {}

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Event")
	bool bAsynchronousEvent = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Targets", meta = (EditCondition = "bOverrideTargetSettings"))
	FSelectedTargetsSettings TargetSettings;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Targets", meta = (InlineEditConditionToggle))
	bool bOverrideTargetSettings = false;

	UPROPERTY(BlueprintReadOnly, Category = "Event")
	TObjectPtr<UBattleContext> BattleContext = nullptr;
};

