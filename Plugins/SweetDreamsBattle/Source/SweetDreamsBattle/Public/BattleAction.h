// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "BattleElement.h"
#include "LevelSequence.h"
#include "BattleAction.generated.h"

class ASweetDreamsBattleManager;

UCLASS()
class SWEETDREAMSBATTLE_API UBattleAction : public UBattleElement
{
	GENERATED_BODY()
	
public:
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	static UBattleAction* CreateLearnAction(AActor* ActionOwner, TSubclassOf<UBattleAction> Action);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	static TArray<UBattleAction*> CreateLearnActions(AActor* ActionOwner, TArray<TSubclassOf<UBattleAction>> Actions);
	// ACTION CONTROL
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void StartAction(bool bUseCooldown);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void StartActionForced(bool bUseCooldown);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void EvaluateCooldown();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void ApplyCooldown();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void UpdateTurnCooldown();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void ResetCooldown();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual float GetPriorityWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action", meta = (DisplayName = "Remove Self From Battle"))
	virtual void RemoveSelfBattle();
	// OWNERSHIP & TARGET
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void ResetAction();
	virtual void ResetSkipAction();
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Action")
	virtual bool IsActionAvailable();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action|Target")
	virtual ETargetType GetTargetType() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action|Target")
	virtual int32 GetTargetAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action|Target")
	virtual bool GetIfIncludeSelf() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual int32 GetActionSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void SetActionSpeed(int32 NewSpeed);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual float GetActionCost() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action", meta = (ReturnDisplayName="Consumed with Success"))
	virtual bool ApplyConsumeCost();
	virtual bool UpdateValidTargets() override;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void LoadRandomTurnTargets();
	//
	virtual bool DamageTargets(TArray<AActor*> Targets, float& PostMitigatedDamage, int32& KilledTargets, float Damage = 100.0f, float ResistenceShred = 0.f, bool bCanBeMitigated = true, bool bApplyCalculations = true, bool bIsAdditionalDamage = false) override;
	virtual float StartAnimation(UAnimSequence* Animation, TArray<AActor*> Targets) override;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void MoveToTarget(AActor* Target, int32 MovementID);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void ReturnToPosition(float Delay = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action", meta = (ReturnDisplayName="Sequence Duration"))
	virtual float PlayLevelSequence(ULevelSequence* Sequence);
	//
	UPROPERTY(BlueprintReadWrite, Category = "Action")
	bool bSkipThis = false;
	UPROPERTY(BlueprintReadWrite, Category = "Action")
	int32 RemovedCount = 0;

protected:

	// ACTION CONTROL
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnActionStart();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnActionEnd();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnMovementComplete(int32 MovementID);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnAnimationComplete(UAnimSequence* Animation);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnSequenceComplete(ULevelSequence* Sequence);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnSequenceBindings(ULevelSequencePlayer* SequencePlayer);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	bool OnAvailabilityRule();
	bool OnAvailabilityRule_Implementation() { return true; }
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void EndAction(float Delay = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void StartNextTurnAction(float Delay = 1.0f);
	//
	FTimerHandle ActionTimer;
	FTimerHandle ActionCooldown;
	UPROPERTY(BlueprintReadWrite, Category = "Action")
	int32 TurnsPassed = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Action")
	bool bIsOnCooldown = false;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	bool bIsAllyAction = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", meta = (DisplayName = "Targets"))
	ETargetType TargetType = ETargetType::Ally;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", meta = (EditCondition = "TargetType==ETargetType::Ally"))
	bool bIncludeSelf = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", meta = (ClampMin = "1"))
	int32 TargetAmount = 1;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ClampMin = "0"))
	float Cost = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bAutoConsumeCost = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bGetManaFromBattleManager = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (DisplayName = "Cooldown (Seconds)", ClampMin = "0", EditCondition = "bTurnBasedAction==false", EditConditionHides))
	float Cooldown = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (DisplayName = "Cooldown (Turns)", ClampMin = "0", EditCondition = "bTurnBasedAction==true", EditConditionHides))
	int32 CooldownTurns = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ClampMin = "0.1", ClampMax = "1"))
	float PriorityWeigth = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ClampMin = "0", ClampMax = "99999", EditCondition = "bTurnBasedAction==true", EditConditionHides))
	int32 ActionSpeed = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bIsDangerous = false;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Restore", meta = (ClampMin = "0"))
	float HealAmount = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Restore", meta = (ClampMin = "0"))
	float ManaRestoreAmount = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Restore", meta = (ClampMin = "0", Units = "%"))
	float HealthVampirism = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Restore", meta = (ClampMin = "0", Units = "%"))
	float ManaVampirism = 0.f;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sequence")
	ULevelSequence* StartingSequence = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Sequence")
	class ALevelSequenceActor* StartingSequenceActor = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sequence")
	bool bWaitSequenceToStart = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bTurnBasedAction = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings", meta = (EditCondition = "bTurnBasedAction==true", EditConditionHides))
	bool bRandomizeTargetsOnStart = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings", meta = (EditCondition = "bTurnBasedAction==true", EditConditionHides))
	bool bOverrideOwnerSpeed = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings", meta = (DisplayName = "Is Last Action when Forced", EditCondition = "bTurnBasedAction==true", EditConditionHides))
	bool bAddedLast = false;
};
