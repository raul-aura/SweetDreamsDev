// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Battle/SweetDreamsBattleManager.h"
#include "TurnBasedBattle.generated.h"

class UBattleInputAction;
class UTurnBasedBattleWidget;

USTRUCT(BlueprintType)
struct FEnemyGroups
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSoftClassPtr<AActor>> EnemyGroup;

	FEnemyGroups() {}
};

UCLASS()
class SWEETDREAMSBATTLE_API ATurnBasedBattle : public AActor
{
	GENERATED_BODY()
	
public:
	ATurnBasedBattle();

protected:
	virtual void BeginPlay() override;

	// BATTLE
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings", meta = (ClampMin = "0"))
	float BattleSpeed = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bHandleDuplicateEnemyNames = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bHandleDuplicateAllyNames = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bUseAlphabeticalSuffix = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIgnoreZForTransform = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float TransformDistanceTolerance = 25.f;

	// UI
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UTurnBasedBattleWidget* TurnBattleWidget;

	// BATTLERS
	UPROPERTY(BlueprintReadOnly, Category = "Battlers")
	TArray<AActor*> AllBattlers;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	USceneComponent* AllyRoot; 
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	USceneComponent* EnemyRoot;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battlers", meta = (DisplayName = "Enemies"))
	TArray<TSoftClassPtr<AActor>> EnemyClasses;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battlers")
	TArray<FTransform> EnemyTransforms;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battlers", meta = (DisplayName = "Allies", EditCondition = "bGetAlliesFromPlayerController==false", EditConditionHides))
	TArray<TSoftClassPtr<AActor>> AllyClasses;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battlers")
	TArray<FTransform> AllyTransforms;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Groups")
	TArray<FEnemyGroups> EnemyGroups;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Groups")
	int32 CurrentEnemyGroup = 0;
	// 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera", meta = (ClampMin = "0"))
	float FirstTurnDelay = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera", meta = (DisplayName = "Camera Blend Delay", ClampMin = "0", ToolTip = "Delay (in seconds) before starting a camera view blend.\n\nThis is automatically called when a Battler starts its input turn."))
	float CameraDelay = 0.8f;
	// TURN
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn", meta = (ClampMin = "0"))
	int32 MaxTurns = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Turn")
	int32 CurrentTurn = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	bool bVictoryWhenMaxTurns = true;
	// ACTION
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	TArray<UBattleElement*> Actions;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Action")
	TSubclassOf<UBattleInputAction> InputActionClass;
	UPROPERTY(BlueprintReadWrite, Category = "Action")
	int32 CurrentAction = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Action")
	AActor* CurrentActionBattler = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", meta = (ClampMin = "0"))
	float ActionDelay = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	bool bAutoLoadAllyActions = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	bool bAutoLoadEnemyActions = true;

public:

	// BATTLE
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void EvaluateTransforms();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void PauseTurnBattle();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void UnpauseTurnBattle();
	// LOAD SPAWN
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void HandleDuplicateNames(const TArray<AActor*>& Battlers);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void GetAlliesFromArray(TArray<TSoftClassPtr<AActor>> NewAllies);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void GetEnemiesFromArray(TArray<TSoftClassPtr<AActor>> NewEnemies);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual TArray<TSoftClassPtr<AActor>> LoadEnemiesGroup(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual TArray<TSoftClassPtr<AActor>> LoadRandomEnemyGroup();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual AActor* SpawnEnemy(TSubclassOf<AActor> EnemyClass, bool bSpawnOnlyIfEmpty = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual AActor* SpawnAlly(TSubclassOf<AActor> AllyClass, bool bSpawnOnlyIfEmpty = true);
	// TRANSFORM
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual FTransform GetFreeAllyTransform() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual FTransform GetFreeEnemyTransform() const;
	// SPEED
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual float GetBattleSpeed() const { return BattleSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void ChangeBattleSpeed(float NewSpeed = 1.0f);
	// UI
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual UTurnBasedBattleWidget* GetTurnBattleWidget() const;
	// GETTERS
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Turn Battle Manager", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ATurnBasedBattle* FindActiveTurnBattle(const UObject* WorldContext, UPARAM(DisplayName="Battle Index") int32& BattleId);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Turn Battle Manager", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ATurnBasedBattle* FindTurnBattleByIndex(const UObject* WorldContext, int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	FString GetBattlerBaseName(const FString& Name);
	// TURNS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void StartTurn();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void LoadTurnActions(TArray<AActor*> Characters, bool bIsAlly);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void AddTurnAction(UBattleElement* Action, bool bIgnoreSpeed = false, int32 IndexToAdd = -1);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager", meta = (ReturnDisplayName = "Found and Removed"))
	virtual bool RemoveTurnAction(UBattleElement* Action, int32& ActionCount);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void StartTurnAction();
	// ACTION
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual bool TurnContainsAction(UBattleElement* Action, int32& Amount) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual bool TurnContainsActionOfClass(TSubclassOf<UBattleElement> Action, int32& Amount) const;
	// ChangeActionOrder()
	// GetAllActionsOfOwner()
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	void OnTurnStarted(int32 Turn);
};
