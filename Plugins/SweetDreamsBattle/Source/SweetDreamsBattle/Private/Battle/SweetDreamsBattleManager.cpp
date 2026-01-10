// Fill out your copyright notice in the Description page of Project Settings.

#include "Battle/SweetDreamsBattleManager.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"

ASweetDreamsBattleManager::ASweetDreamsBattleManager()
{
	PrimaryActorTick.bCanEverTick = true;
	BattleRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Battle Manager"));
	BattleRoot->SetupAttachment(RootComponent);
}

ASweetDreamsBattleManager* ASweetDreamsBattleManager::GetBattleManager(const UObject* WorldContext)
{
	if (!IsValid(WorldContext)) return nullptr;

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull))
	{
		TArray<AActor*> OutActors;
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (AActor* Actor = *It)
			{
				return Cast<ASweetDreamsBattleManager>(Actor);
			}
		}
	}

	return nullptr;
}

void ASweetDreamsBattleManager::AddActorToBattle(AActor* Battler)
{
	RemoveInvalidBattlers();

	if (IsValid(Battler))
	{
		Battlers.AddUnique(Battler);
	}
}

void ASweetDreamsBattleManager::AddActorsToBattle(TArray<AActor*> InBattlers)
{
	RemoveInvalidBattlers();

	for (AActor* Battler : InBattlers)
	{
		if (IsValid(Battler))
		{
			Battlers.AddUnique(Battler);
		}
	}
}

void ASweetDreamsBattleManager::StartBattle()
{
	if (!bIsBattleActive) {
		bIsBattleActive = true;
		OnBattleStart();
	}
}

void ASweetDreamsBattleManager::EndBattle()
{
	if (bIsBattleActive) {
		bIsBattleActive = false;
		OnBattleEnd();

		bIsVictorious = EvaluateBattleVictory();
		if (bIsVictorious)
		{
			OnBattleVictory();
		}
		else
		{
			OnBattleDefeat();
		}
	}
}

bool ASweetDreamsBattleManager::EvaluateBattleVictory_Implementation() const
{
	return false;
}

void ASweetDreamsBattleManager::RemoveInvalidBattlers()
{
	Battlers.RemoveAll([](const TWeakObjectPtr<AActor>& Ptr)
	{
		return !Ptr.IsValid();
	});
}

TArray<AActor*> ASweetDreamsBattleManager::GetBattlers() const
{
	TArray<AActor*> OutBattlers;

	for (const TWeakObjectPtr<AActor>& WeakBattler : Battlers)
	{
		if (AActor* Battler = WeakBattler.Get())
		{
			OutBattlers.Add(Battler);
		}
	}

	return OutBattlers;
}

