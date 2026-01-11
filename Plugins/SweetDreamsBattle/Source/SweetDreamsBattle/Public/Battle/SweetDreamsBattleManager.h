// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/BattleDataTypes.h"
#include "SweetDreamsBattleManager.generated.h"

class USceneComponent;
class UCameraComponent;

UCLASS()
class SWEETDREAMSBATTLE_API ASweetDreamsBattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASweetDreamsBattleManager();

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsBattleManager* GetBattleManager(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void AddActorToBattle(AActor* Battler, bool bRemoveInvalidBattlers = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void AddActorsToBattle(TArray<AActor*> InBattlers, bool bRemoveInvalidBattlers = true);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void StartBattle(AActor* Battler);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void EndBattle();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void EvaluateBattleEnd();

protected:

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	TObjectPtr<USceneComponent> BattleRoot;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleStart();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleEnd();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleVictory();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleDefeat();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	bool EvaluateBattleVictory() const;
	bool EvaluateBattleVictory_Implementation() const;

	UFUNCTION()
	void OnBattlerKilled(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void RemoveInvalidBattlers();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	TArray<AActor*> GetBattlers() const;

	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	bool bIsBattleActive = false;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	bool bIsVictorious = false;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	bool bBattlePaused = false;

	TArray<TWeakObjectPtr<AActor>> Battlers;
	ETeamType VictoriousTeam = ETeamType::None;
	
};
