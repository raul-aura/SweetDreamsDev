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

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static USweetDreamsBattleCore* GetSweetDreamsBattleCore(const UObject* WorldContext);

	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|RPG")
	FOnDifficultySet OnDifficultySet;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Difficulty")
	int32 GetDifficulty() const { return BattleDifficulty; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Difficulty")
	void SetDifficulty(int32 NewDifficulty = 1);

	int32 UpdateLevelsByAvarage(const TArray<AActor*> TargetActors, const TArray<AActor*> AvarageActors);
	int32 GetAvarageLevel(const TArray<AActor*> Actors);
	void OverrideLevels(const TArray<AActor*> Actors, int32 NewLevel = 1);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Difficulty", meta = (ClampMin = "0"))
	int32 BattleDifficulty = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Level", meta = (ClampMin = "0"))
	float LevelAdjustMultiplier = 1.f;
};
