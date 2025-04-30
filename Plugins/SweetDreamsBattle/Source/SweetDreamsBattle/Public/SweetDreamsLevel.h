// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SweetDreamsLevel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpgraded, int32, NewLevel);

UCLASS(ClassGroup = ("SweetDreams"), Blueprintable)
class SWEETDREAMSBATTLE_API USweetDreamsLevel : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|RPG|Level")
	FOnLevelUpgraded OnLevelUpgraded;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	int32 GetCurrentLevel() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	void SetCurrentLevel(int32 NewLevel = 1);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	void AddExperience(float Experience = 1);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	void SetExperience(float Experience = 0, bool bUpdateLevel = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	float GetExperienceRequiredForNextLevel(bool bSubtractCurrentExperience) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	float GetMaxExperienceRequired(bool bSubtractExperienceEarned) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	bool IsAtMaxLevel() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|RPG|Level")
	float GetExperienceRequiredForLevel(int32 Level, bool bIsCumulative, bool bSubtractCurrentExperience) const;
	float GetExperienceRequiredForLevel_Implementation(int32 Level, bool bIsCumulative, bool bSubtractCurrentExperience) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	float GetLevelProgressPercentage() const;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Level", meta = (ClampMin = "0"))
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
	float DiminishingFactor = 0.2f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Experience")
	bool bUseCumulativeExperience = false;
	UPROPERTY(BlueprintReadWrite, Category = "Experience")
	float CurrentExperience = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Experience")
	float TotalExperienceEarned = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	void EvaluateAndUpdateLevel();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Level")
	void UpgradeLevel();
};

