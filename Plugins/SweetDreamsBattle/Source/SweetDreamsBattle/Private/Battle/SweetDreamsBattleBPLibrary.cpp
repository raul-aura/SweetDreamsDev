// Copyright Epic Games, Inc. All Rights Reserved.

#include "Battle/SweetDreamsBattleBPLibrary.h"
#include "Battle/SweetDreamsBattleCore.h"
#include "Battle/SweetDreamsBattle.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Battle/BattleActorComponent.h"

USweetDreamsBattleBPLibrary::USweetDreamsBattleBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

USweetDreamsBattleCore* USweetDreamsBattleBPLibrary::GetSweetDreamsBattleCore(const UObject* WorldContext)
{
	if (UWorld* World = USweetDreamsBPLibrary::GetValidWorld(WorldContext))
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			return GameInstance->GetSubsystem<USweetDreamsBattleCore>();
		}
	}
	return nullptr;
}

bool USweetDreamsBattleBPLibrary::AreTeamsHostile(const ETeamType TeamA, const ETeamType TeamB)
{
	return TeamA != TeamB;
}

TArray<UBattleActorComponent*> USweetDreamsBattleBPLibrary::GetTargetsFromDamageHealResult(const FDamageHealResult& Result)
{
	TArray<FDamageHealTargetResult> LocalTargets = Result.Targets;
	TArray<UBattleActorComponent*> BattleActors;

	for (const FDamageHealTargetResult& LocalTarget : LocalTargets)
	{
		if (IsValid(LocalTarget.Target))
		{
			BattleActors.Add(LocalTarget.Target);
		}
	}

	return BattleActors;
}

int32 USweetDreamsBattleBPLibrary::GetDifficulty(const UObject* WorldContext)
{
	if (USweetDreamsBattleCore* BattleCore = GetSweetDreamsBattleCore(WorldContext))
	{
		return BattleCore->GetDifficulty();
	}
	return INDEX_NONE;
}

void USweetDreamsBattleBPLibrary::SetDifficulty(const UObject* WorldContext, int32 NewDifficulty)
{
	if (USweetDreamsBattleCore* BattleCore = GetSweetDreamsBattleCore(WorldContext))
	{
		return BattleCore->SetDifficulty(NewDifficulty);
	}
}

int32 USweetDreamsBattleBPLibrary::UpdateLevelsByAverage(const UObject* WorldContext, const TArray<AActor*>& TargetActors, const TArray<AActor*>& AvarageActors)
{
	int32 Avarage = GetAverageLevel(WorldContext, AvarageActors);
	OverrideLevels(WorldContext, TargetActors, Avarage);
	return Avarage;
}

int32 USweetDreamsBattleBPLibrary::GetAverageLevel(const UObject* WorldContext, const TArray<AActor*>& Actors)
{
	int32 Avarage = 0;
	/*if (Actors.Num() == 0) return 0;
	for (const AActor* Actor : Actors)
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
	Avarage /= Actors.Num();*/
	return Avarage;
}

void USweetDreamsBattleBPLibrary::OverrideLevels(const UObject* WorldContext, const TArray<AActor*>& Actors, int32 NewLevel)
{
	if (Actors.Num() == 0) return;
	/*for (const AActor* Actor : Actors)
	{
		if (IsValid(Actor))
		{
			UBattlerDataComponent* Data = UBattlerDataComponent::GetBattlerDataComponent(Actor);
			if (IsValid(Data))
			{
				Data->SetLevelNumber(NewLevel);
			}
		}
	}*/
}

float USweetDreamsBattleBPLibrary::IncreaseParameterLinear(UPARAM(ref) float& Parameter, float BaseParameter, float AdditionalParameter, float Multiplier)
{
	Parameter = BaseParameter + (AdditionalParameter * Multiplier);
	return Parameter;
}

float USweetDreamsBattleBPLibrary::IncreaseParameterExponential(UPARAM(ref) float& Parameter, float BaseParameter, float AdditionalParameter, float Multiplier, float Power)
{
	Parameter = BaseParameter + (AdditionalParameter * FMath::LogX(Multiplier, Power));
	return Parameter;
}

float USweetDreamsBattleBPLibrary::IncreaseParameterLogarithmic(UPARAM(ref) float& Parameter, float BaseParameter, float AdditionalParameter, float Multiplier, float LogBase)
{
	if (LogBase <= 1.f)
	{
		LogBase = 2.f;
	}
	float LogFactor = FMath::LogX(LogBase, Multiplier + 1.f);
	Parameter = BaseParameter + (AdditionalParameter * Multiplier * LogFactor);
	return Parameter;
}

float USweetDreamsBattleBPLibrary::IncreaseParameterDiminishing(UPARAM(ref) float& Parameter, float BaseParameter, float AdditionalParameter, float Multiplier, float DiminishFactor)
{
	float DiminishedIncrease = (AdditionalParameter * Multiplier) / (1 + DiminishFactor * Multiplier);
	Parameter = BaseParameter + DiminishedIncrease;
	return Parameter;
}

