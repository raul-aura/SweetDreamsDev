// Copyright Epic Games, Inc. All Rights Reserved.

#include "Battle/SweetDreamsBattleBPLibrary.h"
#include "Battle/SweetDreamsBattleCore.h"
#include "Battle/SweetDreamsBattle.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Battle/BattleActorComponent.h"
#include "Data/BattleElement.h"
#include "Data/BattleEvent.h"

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

UBattleElementData* USweetDreamsBattleBPLibrary::GetRandomBattleElement(const TArray<UBattleElement*>& Elements)
{
	if (Elements.IsEmpty()) return nullptr;

	TArray<UBattleElement*> ForcedElements;
	TArray<TPair<UBattleElement*, float>> WeightedElements;

	float TotalWeight = 0.f;

	for (UBattleElement* Element : Elements)
	{
		if (!IsValid(Element)) continue;

		const TArray<UBattleEvent*> Events = Element->GetBattleEvents();

		bool bForced = false;
		float BestWeight = 0.f;

		if (bForced)
		{
			ForcedElements.Add(Element);
			continue;
		}

		if (BestWeight > 0.f)
		{
			WeightedElements.Add({ Element, BestWeight });
			TotalWeight += BestWeight;
		}
	}

	if (!ForcedElements.IsEmpty())
	{
		const int32 Index = FMath::RandRange(0, ForcedElements.Num() - 1);
		return ForcedElements[Index]->BattleElementData;
	}

	if (!WeightedElements.IsEmpty())
	{
		const float Roll = FMath::FRandRange(0.f, TotalWeight);

		float Accumulated = 0.f;

		for (const TPair<UBattleElement*, float>& Pair : WeightedElements)
		{
			Accumulated += Pair.Value;

			if (Roll <= Accumulated)
			{
				return Pair.Key->BattleElementData;
			}
		}

		return WeightedElements.Last().Key->BattleElementData;
	}

	const int32 Index = FMath::RandRange(0, Elements.Num() - 1);
	return Elements[Index] ? Elements[Index]->BattleElementData : nullptr;
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

