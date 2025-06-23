// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/SweetDreamsLevel.h"
#include "Battle/SweetDreamsBattleBPLibrary.h"

int32 USweetDreamsLevel::GetCurrentLevel() const
{
	return CurrentLevel;
}

void USweetDreamsLevel::SetCurrentLevel(int32 NewLevel)
{
	CurrentLevel = FMath::Clamp(NewLevel, StartingLevel, MaxLevel);
    TotalExperienceEarned = GetExperienceRequiredForLevel(CurrentLevel, true, false);
    CurrentExperience = bUseCumulativeExperience ? TotalExperienceEarned : 0.f;
}

void USweetDreamsLevel::AddExperience(float Experience)
{
	if (Experience <= 0) return;
    const float MaxExperience = GetMaxExperienceRequired(false);
    TotalExperienceEarned = FMath::Min(TotalExperienceEarned + Experience, MaxExperience);
    if (bUseCumulativeExperience)
    {
        CurrentExperience = TotalExperienceEarned;
    }
    else
    {
        CurrentExperience = FMath::Min(CurrentExperience + Experience, MaxExperience);
    }
	EvaluateAndUpdateLevel();
}

void USweetDreamsLevel::SetExperience(float Experience, bool bUpdateLevel)
{
	if (Experience <= 0) return;
    TotalExperienceEarned = FMath::Min(Experience, GetMaxExperienceRequired(false));
    CurrentExperience = TotalExperienceEarned;
	if (bUpdateLevel) EvaluateAndUpdateLevel();
}

float USweetDreamsLevel::GetExperienceRequiredForNextLevel(bool bSubtractCurrentExperience) const
{
    if (IsAtMaxLevel()) return 0.f;
	float ExperienceRequired = GetExperienceRequiredForLevel(CurrentLevel + 1, bUseCumulativeExperience, false);
	if (bUseCumulativeExperience)
	{
		ExperienceRequired -= GetExperienceRequiredForLevel(CurrentLevel, true, false);
	}
	return bSubtractCurrentExperience ? ExperienceRequired - CurrentExperience : ExperienceRequired;
}

float USweetDreamsLevel::GetMaxExperienceRequired(bool bSubtractExperienceEarned) const
{
    const float ExperienceRequired = GetExperienceRequiredForLevel(MaxLevel, true, false);
    return bSubtractExperienceEarned ? ExperienceRequired - TotalExperienceEarned : ExperienceRequired;
}

bool USweetDreamsLevel::IsAtMaxLevel() const
{
    return CurrentLevel >= MaxLevel;
}

float USweetDreamsLevel::GetExperienceRequiredForLevel_Implementation(int32 Level, bool bIsCumulative, bool bSubtractCurrentExperience) const
{
    if (Level < StartingLevel) return 0.f;
    if (Level == StartingLevel) return BaseExperience;
    float TotalExperience = 0.f;
    float LevelExperience = BaseExperience;
    for (int32 i = StartingLevel + 1; i <= Level; ++i)
    {
        LevelExperience = USweetDreamsBattleBPLibrary::IncreaseParameterDiminishing(LevelExperience, BaseExperience, IncrementalExperience, i, DiminishingFactor);
        if (bIsCumulative)
        {
            TotalExperience += LevelExperience;
        }
    }
    const float ExperienceRequired = bIsCumulative ? TotalExperience : LevelExperience;
    return bSubtractCurrentExperience ? ExperienceRequired - CurrentExperience : ExperienceRequired;
}

float USweetDreamsLevel::GetLevelProgressPercentage() const
{
    if (IsAtMaxLevel()) return 1.f;
    const float NextLevelExp = GetExperienceRequiredForNextLevel(false);
    return FMath::Clamp(CurrentExperience / NextLevelExp, 0.f, 1.f);
}

void USweetDreamsLevel::EvaluateAndUpdateLevel()
{
    while (!IsAtMaxLevel())
    {
        const float NextLevelExp = GetExperienceRequiredForNextLevel(false);
        if (NextLevelExp <= 0.f || CurrentExperience < NextLevelExp) break;
        UpgradeLevel();
        if (!bUseCumulativeExperience && !IsAtMaxLevel())
        {
            CurrentExperience = FMath::Max(CurrentExperience - NextLevelExp, 0.f);
        }
    }
}

void USweetDreamsLevel::UpgradeLevel()
{
	CurrentLevel = FMath::Clamp(++CurrentLevel, StartingLevel, MaxLevel);
	OnLevelUpgraded.Broadcast(CurrentLevel);
}
