// Fill out your copyright notice in the Description page of Project Settings.

#include "Battle/SweetDreamsBattleManager.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "Battle/BattleActorComponent.h"

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

void ASweetDreamsBattleManager::AddActorToBattle(AActor* Battler, bool bRemoveInvalidBattlers)
{
	if (bRemoveInvalidBattlers)
	{
		RemoveInvalidBattlers();
	}

	if (IsValid(Battler) && !Battlers.FindByKey(Battler))
	{
		if (UBattleActorComponent* Component = UBattleActorComponent::GetBattleActorComponent(Battler))
		{
			Component->OnKill.AddUniqueDynamic(this, &ASweetDreamsBattleManager::OnBattlerKilled);
		}

		Battlers.AddUnique(Battler);
	}
}

void ASweetDreamsBattleManager::AddActorsToBattle(TArray<AActor*> InBattlers, bool bRemoveInvalidBattlers)
{
	if (bRemoveInvalidBattlers)
	{
		RemoveInvalidBattlers();
	}

	for (AActor* Battler : InBattlers)
	{
		AddActorToBattle(Battler, false);
	}
}

void ASweetDreamsBattleManager::StartBattle(AActor* Battler)
{
	AddActorToBattle(Battler);

	if (Battlers.IsEmpty())
	{
		return;
	}

	if (!bIsBattleActive) {
		bIsBattleActive = true;
		OnBattleStart();
	}
}

void ASweetDreamsBattleManager::EndBattle()
{
	if (bIsBattleActive && !Battlers.IsEmpty()) {
		bIsBattleActive = false;

		// TO DO: unbind delegates from battlers and exit from combat

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

void ASweetDreamsBattleManager::EvaluateBattleEnd()
{
	TSet<ETeamType> AliveTeams;

	if (Battlers.IsEmpty())
	{
		return;
	}

	for (const TWeakObjectPtr<AActor>& WeakBattler : Battlers)
	{
		if (AActor* Battler = WeakBattler.Get())
		{
			if (UBattleActorComponent* Component = UBattleActorComponent::GetBattleActorComponent(Battler))
			{
				if (Component->IsAlive())
				{
					AliveTeams.Add(Component->GetTeam());

					if (AliveTeams.Num() > 1)
					{
						return;
					}
				}
			}
		}
	}

	if (!AliveTeams.IsEmpty())
	{
		for (const ETeamType& Team : AliveTeams)
		{
			VictoriousTeam = Team;
			break;
		}
	}

	EndBattle();
}

bool ASweetDreamsBattleManager::EvaluateBattleVictory_Implementation() const
{
	return VictoriousTeam == ETeamType::Player;
}

void ASweetDreamsBattleManager::OnBattlerKilled(AActor* Target)
{
	if (Battlers.IsEmpty())
	{
		return;
	}

	// TO DO: unbind delegates and exit from combat

	if (Battlers.FindByKey(Target))
	{
		EvaluateBattleEnd();
	}
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

