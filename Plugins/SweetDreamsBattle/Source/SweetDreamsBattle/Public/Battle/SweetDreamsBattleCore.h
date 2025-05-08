// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SweetDreamsBattleCore.generated.h"

class ASweetDreamsBattleManager;

UCLASS(Category = "SweetDreams|RPG")
class SWEETDREAMSBATTLE_API USweetDreamsBattleCore : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USweetDreamsBattleCore();
	virtual ~USweetDreamsBattleCore();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// DIFFICULTY
	int32 GetDifficulty() const;
	void SetDifficulty(int32 NewDifficulty = 1);

	//
	void RegisterBattleListener(const UObject* Listener);
	void UnregisterBattleListener(const UObject* Listener);
	void OnBattleStart(ASweetDreamsBattleManager* BattleReference);
	void OnBattleEnd(ASweetDreamsBattleManager* BattleReference);
	void OnTurnStart();
	void OnTurnEnd();
	void OnActionStart();
	void OnActionEnd();
	void OnDamageDealt();
	void OnDamageReceived();
	void OnKilled();
	void OnRevived();
	void OnHealthChanged();
	void OnHealed();
	void OnHealingGranted();
	void OnManaChanged();
	void OnManaRestored();
	void OnManaConsumed();
	void OnManaRestoreGranted();

protected:
	UPROPERTY()
	int32 BattleDifficulty = 0;

	TArray<TWeakObjectPtr<UObject>> BattleListener;
};
