// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SweetDreamsLevel.h"
#include "Engine/DataTable.h"
#include "BattlerDataComponent.generated.h"

USTRUCT(BlueprintType)
struct SWEETDREAMSBATTLE_API FActionData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBattleAction> Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 Level = 1;
};


UCLASS(ClassGroup = ("SweetDreams"), Blueprintable, meta = (BlueprintSpawnableComponent))
class SWEETDREAMSBATTLE_API UBattlerDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBattlerDataComponent();

protected:
	// PARAMS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battler")
	FText BattlerName = FText::FromString(TEXT("Battler Actor"));
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battler")
	TArray<UTexture2D*> Portraits;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item RPG")
	TSubclassOf<USweetDreamsLevel> LevelClass = USweetDreamsLevel::StaticClass();
	UPROPERTY(BlueprintReadWrite, Category = "Item RPG")
	USweetDreamsLevel* LevelObject = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "1"))
	float BaseHealth = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0"))
	float BaseMana = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0", EditCondition = "bOverrideStartingMana"))
	float StartingMana = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (InlineEditConditionToggle))
	bool bOverrideStartingMana = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0"))
	float BaseForce = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0"))
	float BaseResistence = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0"))
	int32 BaseSpeed = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0", ClampMax = "999"))
	int32 BaseActionsPerTurn = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0", ClampMax = "999"))
	int32 AdditionalActions = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0", ClampMax = "999"))
	int32 AdditionalLives = 0;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params")
	bool bIsAbleToAct = true;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float MaxHealth = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float MaxMana = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Health = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Mana = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Force = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Resistence = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	int32 Speed = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	int32 CurrentLives = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	bool bIsDead = false;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	bool bIsInBattle = false;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	int32 ActionCount = 0;
	// ELEMENTS
	UPROPERTY(EditAnywhere, Category = "Battle")
	TArray<UBattleElement*> AllElements;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle", meta = (DisplayName = "Actions"))
	TArray<FActionData> ActionClasses;
	UPROPERTY(BlueprintReadWrite, Category = "Battle")
	TArray<UBattleAction*> Actions;
	UPROPERTY(BlueprintReadOnly, Category = "States")
	TArray<class UBattleState*> AllStates;
	// MULTIPLIERS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%", ClampMin = "0"))
	float ForceMultiplier = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%", ClampMin = "0"))
	float ResistenceMultiplier = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%", ClampMin = "0"))
	float SpeedMultiplier = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%", ClampMin = "0"))
	float DamageDealtMultiplier = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%", ClampMin = "0"))
	float DamageReceivedMultiplier = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%", ClampMin = "0"))
	float HealMultiplier = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%", ClampMin = "0"))
	float ManaRestoreMultiplier = 100;

public:	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Battler Data")
	static UBattlerDataComponent* GetBattlerDataComponent(const AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	ABattleCharacter* GetBattlerOwner() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual FText GetCharacterName() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void SetCharacterName(FText NewName);
	// LEVEL
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	USweetDreamsLevel* GetLevelObject() const { return LevelObject; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	int32 GetLevelNumber() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	void SetLevelNumber(int32 NewLevel = 1);
	// PARAMS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	void UpdateParametersByLevel(int32 NewLevel);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Battler Data", meta = (ReturnDisplayName="Current Health"))
	virtual float GetHealth(float& MaximumHealth, float Multiplier = 100.f);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Battler Data", meta = (ReturnDisplayName = "Current Mana"))
	virtual float GetMana(float& MaximumMana, float Multiplier = 100.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data", meta = (ReturnDisplayName = "Force"))
	virtual float GetForce(float Multiplier = 100.f) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data", meta = (ReturnDisplayName = "Resistence"))
	virtual float GetResistence(float Multiplier = 100.f) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data", meta = (ReturnDisplayName = "Speed"))
	virtual int32 GetSpeed(float Multiplier = 100.f) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data", meta = (ReturnDisplayName = "Additional Speed"))
	virtual int32 GetAdditionalActions() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual int32 GetBaseActionsPerTurn() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual int32 GetActionsPerTurn() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual bool GetIsAbleToAct() const { return bIsAbleToAct; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void SetIsAbleToAct(bool bNewAbility);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateForceValue(float Value = 10.f, float Percentage = 0.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateResistenceValue(float Value = 10.f, float Percentage = 0.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual int32 UpdateSpeedValue(float Value = 10.f, float Percentage = 0.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateParameter(float& Parameter, float Value, float Percentage);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual int32 UpdateAdditionalActions(int32 Value = 1);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual int32 UpdateAdditionalLives(int32 Value = 1);
	// MANA & HEALTH CONTROL
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateHealthValue(float Value = 10.f, float Percentage = 0.f, bool bUpdateCurrentHealth = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateManaValue(float Value = 10.f, float Percentage = 0.f, bool bUpdateCurrentMana = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float GetHealthPercentage() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float GetManaPercentage() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float ReceiveDamage(float Damage, float ResistenceShred, bool bCanBeMitigated, AActor* DamageInstigator, bool bIsAdditionalDamage);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float ReceiveHeal(float Heal);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float ReceiveManaConsume(float Consume);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float ReceiveManaRestore(float Restore);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float OnMitigateDamage(float Damage, float ResistenceShred = 0.f);
	float OnMitigateDamage_Implementation(float Damage, float ResistenceShred = 0.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	void Kill(AActor* KillInstigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	void Revive(float HealthRestore = 100.f, float ManaRestore = 100.f); 
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual bool IsDead() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual bool IsInBattle() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void SetInBattle(ASweetDreamsBattleManager* BattleReference, bool bNewIsInBattle = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual TArray<UBattleElement*> GetAllElements() const;
	// STATES
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void AddState(TSubclassOf<UBattleState> StateToAdd, UObject* StateInstigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void AddStates(TArray<TSubclassOf<UBattleState>> StatesToAdd, UObject* StateInstigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void RemoveState(TSubclassOf<UBattleState> StateToRemove);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void RemoveStates(TArray<TSubclassOf<UBattleState>> StatesToRemove);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual int32 RemoveAllStates(bool bIncludePositive = true, bool bIncludeNegative = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual UBattleState* GetStateOfClass(TSubclassOf<UBattleState> StateClass) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual TArray<UBattleState*> GetAllStates() const;
	// ACTIONS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void CreateActions();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void AddAction(UBattleAction* ActionToAdd);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void AddActions(TArray<UBattleAction*> ActionsToAdd);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual UBattleAction* LearnAction(TSubclassOf<UBattleAction> ActionToLearn);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual TArray<UBattleAction*> LearnActions(TArray<TSubclassOf<UBattleAction>> ActionsToLearn);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void RemoveAction(TSubclassOf<UBattleAction> ActionToRemove);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void RemoveActions(TArray<TSubclassOf<UBattleAction>> ActionsToRemove);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual UBattleAction* GetRandomAction() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual bool GetAllAvailableActions(TArray<UBattleAction*>& AvailableActions) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual TArray<UBattleAction*> GetAllActions() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void UpdateTurnActionsCooldown();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void ResetActions();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	void IncrementActionCount();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	void ResetActionCount();
	// MULTIPLIERS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float GetForceMultiplier() const { return FMath::Max(ForceMultiplier, 0.f); }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float GetResistenceMultiplier() const { return FMath::Max(ResistenceMultiplier, 0.f); }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float GetSpeedMultiplier() const { return FMath::Max(SpeedMultiplier, 0.f); }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float GetDamageDealtMultiplier() const { return FMath::Max(DamageDealtMultiplier, 0.f); }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float GetDamageReceivedMultiplier() const { return FMath::Max(DamageReceivedMultiplier, 0.f); }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float GetHealMultiplier() const { return FMath::Max(HealMultiplier, 0.f); }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float GetManaRestoreMultiplier() const { return FMath::Max(ManaRestoreMultiplier, 0.f); }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateForceMultiplier(float Value = 10.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateResistenceMultiplier(float Value = 10.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateSpeedMultiplier(float Value = 10.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateDamageDealtMultiplier(float Value = 10.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateDamageReceivedMultiplier(float Value = 10.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateHealMultiplier(float Value = 10.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateManaRestoreMultiplier(float Value = 10.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual float UpdateMultiplier(float& Multiplier, float Value = 10.f);
};
