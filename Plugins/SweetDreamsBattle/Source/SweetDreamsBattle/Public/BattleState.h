// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleElement.h"
#include "BattleState.generated.h"

UENUM(BlueprintType)
enum class EStateLifetime : uint8
{
	Permanent UMETA(DisplayName = "Permanent (Until Cleansed)"),
	Turn UMETA(DisplayName = "On Turn Start"),
	Action UMETA(DisplayName = "On Action End"),
	Second UMETA(DisplayName = "Seconds"),
};

UENUM(BlueprintType)
enum class EStateType : uint8
{
	NONE,
	POSITIVE,
	NEGATIVE,
};

UCLASS()
class SWEETDREAMSBATTLE_API UBattleState : public UBattleElement
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Battle State")
	UObject* StateInstigator = nullptr;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual UObject* GetStateInstigator() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual TArray<UObject*> GetInstigatorAsArray() const;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle State")
	TArray<TSubclassOf<UBattleAction>> TriggerableActions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle State")
	EStateType StateType;

	// STACKS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stacks")
	bool bIsStackable = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stacks")
	bool bCanConsumeStacks = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stacks", meta = (DisplayName = "Auto Consume Stacks on Lifetime"))
	bool bCanConsumeStacksOnLifetime = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stacks")
	bool bRemoveOnZeroStacks = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stacks", meta = (ClampMin = "1"))
	int32 InitialStacks = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stacks", meta = (ClampMin = "1"))
	int32 MaxStacks = 1;
	UPROPERTY(BlueprintReadOnly, Category = "Stacks")
	int32 Stacks = 0;

	// LIFETIME
	UPROPERTY(BlueprintReadOnly, Category = "Lifetime")
	bool bAlreadyAppliedOnce = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lifetime")
	EStateLifetime Lifetime = EStateLifetime::Permanent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lifetime", meta = (EditCondition = "Lifetime==EStateLifetime::Turn", ClampMin = "0"))
	int32 TurnsToEnd = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lifetime", meta = (EditCondition = "Lifetime==EStateLifetime::Action", ClampMin = "0"))
	int32 ActionsToEnd = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lifetime", meta = (EditCondition = "Lifetime==EStateLifetime::Second", ClampMin = "0"))
	float SecondsToEnd = 0.1f;
	UPROPERTY(BlueprintReadWrite, Category = "Lifetime", meta = (DisplayName = "Lifetime (Turns/Actions)"))
	int32 IntLifetime = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Lifetime", meta = (DisplayName = "Lifetime (Seconds)"))
	float FloatLifetime = 0.f;

	// PARAMS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	float BaseHealth = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params", meta = (Units = "%"))
	float BaseHealthPercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	float BaseMana = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params", meta = (Units = "%"))
	float BaseManaPercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	float BaseForce = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params", meta = (Units = "%"))
	float BaseForcePercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	float BaseResistence = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params", meta = (Units = "%"))
	float BaseResistencePercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	int32 BaseSpeed = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params", meta = (Units = "%"))
	float BaseSpeedPercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	int32 BaseActions = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	int32 BaseLives = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Health = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float HealthPercentage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Mana = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float ManaPercentage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Force = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float ForcePercentage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Resistence = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float ResistencePercentage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	int32 Speed = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float SpeedPercentage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	int32 Actions = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	int32 Lives = 0;
	// MULTIPLIERS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float ForceMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float ResistenceMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float SpeedMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float DamageDealtMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float DamageReceivedMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float HealMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float ManaRestoreMultiplier = 0;

public:
	virtual void PostInitProperties() override;
	// EVENTS
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	void OnApplied();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	void OnReapplied();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	void OnRemoved();
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual bool IsTriggerableAction(UBattleAction* Action) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void ApplyState(UObject* Instigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void ResetLifetime();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void AddStacks(int32 Amount = 1);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void ConsumeStacks(int32 Amount = 1);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual bool IsMaxStacks() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void ConsumeLifetime(EStateLifetime LifetimeToConsume = EStateLifetime::Action);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void RemoveState();
	// GETTERS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual int32 GetRemainingLifetime() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual float GetRemainingLifetimeSeconds() const;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	FText GetDurationAsText() const;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	FText GetInstigatorAsText() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	EStateType GetStateType() const { return StateType; }
	// PARAMS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Item")
	virtual void ApplyParams();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Item")
	virtual void RemoveParams();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Item")
	virtual void RecalculateParams();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	void OnUpdateParams();
};
