// Fill out your copyright notice in the Description page of Project Settings.

#include "Battle/SweetDreamsBattleManager.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"

ASweetDreamsBattleManager::ASweetDreamsBattleManager()
{
	PrimaryActorTick.bCanEverTick = true;
	BattleRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Battle Manager"));
	BattleRoot->SetupAttachment(RootComponent);

	BattleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Battle Camera"));
	BattleCamera->SetupAttachment(BattleRoot);
}

bool ASweetDreamsBattleManager::StartBattleByID(const UObject* WorldContext, FName inID)
{
	bool bIsActive = false;
	ASweetDreamsBattleManager* Battle = FindBattleByID(WorldContext, inID, bIsActive);
	if (IsValid(Battle))
	{
		return Battle->StartBattle();
	}
	return false;
}

ASweetDreamsBattleManager* ASweetDreamsBattleManager::FindBattleByID(const UObject* WorldContext, FName inID, bool& bIsActive)
{
	if (!IsValid(WorldContext)) return nullptr;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull)) 
	{
		TArray<AActor*> OutActors;
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (AActor* Actor = *It)
			{
				ASweetDreamsBattleManager* BattleManager = Cast<ASweetDreamsBattleManager>(Actor);
				if (BattleManager && BattleManager->BattleID == inID)
				{
					bIsActive = BattleManager->bIsBattleActive;
					return BattleManager;
				}
			}
		}
	}
	return nullptr;
}

ASweetDreamsBattleManager* ASweetDreamsBattleManager::GetActiveBattle(const UObject* WorldContext, FName& inBattleID)
{
	inBattleID = TEXT("null");
	if (!IsValid(WorldContext)) return nullptr;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull))
	{
		TArray<AActor*> OutActors;
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (AActor* Actor = *It)
			{
				ASweetDreamsBattleManager* BattleManager = Cast<ASweetDreamsBattleManager>(Actor);
				if (BattleManager && BattleManager->bIsBattleActive)
				{
					inBattleID = BattleManager->BattleID;
					return BattleManager;
				}
			}
		}
	}
	return nullptr;
}

bool ASweetDreamsBattleManager::StartBattle()
{
	if (!bIsBattleActive) {
		bIsBattleActive = true;
		OnBattleStart();
	}
	return false;
}

bool ASweetDreamsBattleManager::EndBattle()
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
	return false;
}

bool ASweetDreamsBattleManager::EvaluateBattleVictory() const
{
	return false;
}

