// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SweetDreamsLevel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelUpgraded);

UCLASS(ClassGroup = ("SweetDreams"), Blueprintable)
class SWEETDREAMSBATTLE_API USweetDreamsLevel : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|RPG|Level")
	FOnLevelUpgraded OnLevelUpgraded;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	int32 GetCurrentLevel() const { return CurrentLevel; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	void OverrideCurrentLevel(int32 NewLevel = 1);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	void AddExperience(float Experience);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	void SetExperience(float Experience);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Level")
	float GetExperienceRequiredForNextLevel();
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Level")
	float GetMaxExperienceRequired();
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Sweet Dreams|RPG|Level")
	float GetExperienceRequiredForLevel(int32 Level);
	float GetExperienceRequiredForLevel_Implementation(int32 Level);

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Level", meta = (ClampMin = "1"))
	int32 StartingLevel = 1;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Level")
	int32 MaxLevel = 99;
	UPROPERTY(BlueprintReadWrite, Category = "Level")
	int32 CurrentLevel = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Experience", meta = (ClampMin = "0"))
	float BaseExperience = 100;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Experience", meta = (ClampMin = "1"))
	float IncrementalExperience = 200;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Experience", meta = (ClampMin = "0"))
	float QuadraticFactor = 0.2f;
	UPROPERTY(BlueprintReadWrite, Category = "Experience")
	float CurrentExperience = 0;

	UFUNCTION(BlueprintCallable)
	void EvaluateAndUpdateLevel();
	UFUNCTION(BlueprintCallable)
	void UpgradeLevel();
};

