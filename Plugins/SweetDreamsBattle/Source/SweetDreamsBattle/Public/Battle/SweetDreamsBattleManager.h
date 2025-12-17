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

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static bool StartBattleByID(const UObject* WorldContext, FName inID);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsBattleManager* FindBattleByID(const UObject* WorldContext, FName inID, bool& bIsActive);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsBattleManager* GetActiveBattle(const UObject* WorldContext, FName& inBattleID);

	bool StartBattle();
	bool EndBattle();
	bool EvaluateBattleVictory() const;

protected:

	// COMPONENTS
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	TObjectPtr<UCameraComponent> BattleCamera;
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Manager")
	FName BattleID = TEXT("Battle-0");
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	bool bIsBattleActive = false;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	bool bIsVictorious = true;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	bool bBattlePaused = false;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	TArray<TObjectPtr<AActor>> Battlers;
	
};
