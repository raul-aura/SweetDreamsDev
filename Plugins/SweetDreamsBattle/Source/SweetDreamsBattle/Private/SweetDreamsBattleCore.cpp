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

USweetDreamsBattleCore* USweetDreamsBattleCore::GetSweetDreamsBattleCore(const UObject* WorldContext)
{
	USweetDreamsBattleCore* BattleCore = nullptr;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(WorldContext) && !IsValid(World))
	{
		return BattleCore;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		BattleCore = GameInstance->GetSubsystem<USweetDreamsBattleCore>();
	}
	return BattleCore;
}

void USweetDreamsBattleCore::SetDifficulty(int32 NewDifficulty)
{
	BattleDifficulty = NewDifficulty;
	OnDifficultySet.Broadcast(BattleDifficulty);
}

int32 USweetDreamsBattleCore::UpdateLevelsByAvarage(const TArray<AActor*> TargetActors, const TArray<AActor*> AvarageActors)
{
	int32 Avarage = GetAvarageLevel(AvarageActors);
	OverrideLevels(TargetActors, Avarage);
	return Avarage;
}

int32 USweetDreamsBattleCore::GetAvarageLevel(const TArray<AActor*> Actors)
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
