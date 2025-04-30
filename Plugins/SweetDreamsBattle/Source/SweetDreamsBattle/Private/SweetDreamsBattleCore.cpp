// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsBattleCore.h"
#include "BattlerDataComponent.h"
#include "SweetDreamsLevel.h"

USweetDreamsBattleCore::USweetDreamsBattleCore()
{
}

USweetDreamsBattleCore::~USweetDreamsBattleCore()
{
}

void USweetDreamsBattleCore::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USweetDreamsBattleCore::SetDifficulty(int32 NewDifficulty)
{
	BattleDifficulty = NewDifficulty;
	OnDifficultySet.Broadcast(BattleDifficulty);
}

int32 USweetDreamsBattleCore::UpdateLevelsByAverage(const TArray<AActor*> TargetActors, const TArray<AActor*> AvarageActors)
{
	int32 Avarage = GetAverageLevel(AvarageActors);
	OverrideLevels(TargetActors, Avarage);
	return Avarage;
}

int32 USweetDreamsBattleCore::GetAverageLevel(const TArray<AActor*> Actors)
{
	int32 Avarage = 0;
	if (Actors.Num() == 0) return 0;
	for (AActor* Actor : Actors)
	{
		if (IsValid(Actor))
		{
			UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(Actor);
			if (IsValid(Data))
			{
				Avarage += Data->GetLevelNumber();
			}
		}
	}
	Avarage /= Actors.Num();
	return Avarage;
}

void USweetDreamsBattleCore::OverrideLevels(const TArray<AActor*> Actors, int32 NewLevel)
{
	if (Actors.Num() == 0) return;
	for (AActor* Actor : Actors)
	{
		if (IsValid(Actor))
		{
			UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(Actor);
			if (IsValid(Data))
			{
				Data->SetLevelNumber(NewLevel);
			}
		}
	}
}
