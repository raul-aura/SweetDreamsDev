// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/BattleDataTypes.h"
#include "SweetDreamsBattleManager.generated.h"

class USceneComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleDelegate);

UCLASS()
class SWEETDREAMSBATTLE_API ASweetDreamsBattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASweetDreamsBattleManager();

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsBattleManager* GetBattleManager(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void InitiateCombat(UBattleActorComponent* Battler);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager", meta = (DisplayName = "Initiate Combat (Multiple)"))
	void InitiateCombatMultiple(TArray<UBattleActorComponent*> InBattlers);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void AddBattler(UBattleActorComponent* Battler);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void AddMultipleBattlers(TArray<UBattleActorComponent*> InBattlers);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void RemoveBattler(UBattleActorComponent* Battler, bool bExitFromCombat = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void RemoveMultipleBattlers(TArray<UBattleActorComponent*> InBattlers, bool bExitFromCombat = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void ClearBattlers(bool bExitFromCombat = true);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void StartBattle();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void EndBattle();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void EvaluateBattleEnd();

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	TArray<UBattleActorComponent*> GetBattlers(ETeamType TeamFilter) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	TArray<UBattleActorComponent*> GetAliveBattlers(ETeamType TeamFilter) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	TArray<UBattleActorComponent*> GetLastBattleBattlers(ETeamType TeamFilter) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	TArray<AActor*> GetBattlersActors(ETeamType TeamFilter) const;

	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|RPG|Battle Manager")
	FOnBattleDelegate OnBattleStarted;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|RPG|Battle Manager")
	FOnBattleDelegate OnBattleEnded;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	TObjectPtr<USceneComponent> BattleRoot;

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleEnd();
	UFUNCTION(BlueprintImplementableEvent,Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleVictory();
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleDefeat();
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattlerAdded(UBattleActorComponent* Battler);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattlerRemoved(UBattleActorComponent* Battler);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattlerKilled(UBattleActorComponent* Battler);
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|RPG|Battle Manager")
	bool EvaluateBattleVictory() const;
	bool EvaluateBattleVictory_Implementation() const;
	
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void RemoveInvalidBattlers();

	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	bool bIsBattleActive = false;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	bool bIsVictorious = false;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	bool bBattlePaused = false;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	ETeamType VictoriousTeam = ETeamType::None;

	TArray<TWeakObjectPtr<UBattleActorComponent>> Battlers;
	TArray<TWeakObjectPtr<UBattleActorComponent>> LastBattleBattlers;
	
private:

	UFUNCTION()
	void OnBattlerKilled_Internal(UBattleActorComponent* Battler);

	void BindFunctionsToBattler(UBattleActorComponent* Battler);
	void UnbindFunctionsFromBattler(UBattleActorComponent* Battler);
};
