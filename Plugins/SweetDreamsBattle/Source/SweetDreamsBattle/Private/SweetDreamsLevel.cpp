// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsLevel.h"


void USweetDreamsLevel::OverrideCurrentLevel(int32 NewLevel)
{
	CurrentLevel = FMath::Clamp(NewLevel, StartingLevel, MaxLevel);
	CurrentExperience = GetExperienceRequiredForLevel(CurrentLevel);
}

void USweetDreamsLevel::AddExperience(float Experience)
{
	if (Experience <= 0) return;
	CurrentExperience += Experience;
	EvaluateAndUpdateLevel();
}

void USweetDreamsLevel::SetExperience(float Experience)
{
	if (Experience <= 0) return;
	CurrentExperience = Experience;
	EvaluateAndUpdateLevel();
}

float USweetDreamsLevel::GetExperienceRequiredForNextLevel()
{
	if (CurrentLevel == MaxLevel) return 0.f;
	return GetExperienceRequiredForLevel(CurrentLevel + 1) - GetExperienceRequiredForLevel(CurrentLevel);
}

float USweetDreamsLevel::GetMaxExperienceRequired()
{
	return GetExperienceRequiredForLevel(MaxLevel);
}

float USweetDreamsLevel::GetExperienceRequiredForLevel_Implementation(int32 Level)
{
	if (Level == StartingLevel) return BaseExperience;
	return BaseExperience + (IncrementalExperience * (Level - 1)) + (QuadraticFactor * FMath::Square(Level - 1));
}

void USweetDreamsLevel::EvaluateAndUpdateLevel()
{
	while (CurrentExperience >= GetExperienceRequiredForNextLevel() && CurrentLevel < MaxLevel)
	{
		UpgradeLevel();
	}
}

void USweetDreamsLevel::UpgradeLevel()
{
	CurrentLevel = FMath::Clamp(++CurrentLevel, StartingLevel, MaxLevel);
	OnLevelUpgraded.Broadcast();
}
