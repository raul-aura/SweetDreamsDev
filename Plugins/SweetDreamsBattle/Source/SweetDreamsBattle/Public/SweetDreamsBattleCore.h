// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SweetDreamsBattleCore.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultySet, int32, Difficulty);

UCLASS(Category = "SweetDreams|RPG")
class SWEETDREAMSBATTLE_API USweetDreamsBattleCore : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USweetDreamsBattleCore();
	~USweetDreamsBattleCore();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|RPG")
	FOnDifficultySet OnDifficultySet;
	UFUNCTION()
	int32 GetDifficulty() const { return BattleDifficulty; }
	UFUNCTION()
	void SetDifficulty(int32 NewDifficulty = 1);

	UFUNCTION()
	int32 UpdateLevelsByAverage(const TArray<AActor*> TargetActors, const TArray<AActor*> AvarageActors);
	UFUNCTION()
	int32 GetAverageLevel(const TArray<AActor*> Actors);
	UFUNCTION()
	void OverrideLevels(const TArray<AActor*> Actors, int32 NewLevel = 1);

protected:
	UPROPERTY()
	int32 BattleDifficulty = 0;
};
