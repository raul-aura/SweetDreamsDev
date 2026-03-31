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
		for (TActorIterator<ASweetDreamsBattleManager> It(World); It; ++It)
		{
			return *It;
		}
	}

	return nullptr;
}

void ASweetDreamsBattleManager::InitiateCombat(UBattleActorComponent* Battler)
{
	AddBattler(Battler);

	if (!bIsBattleActive && Battlers.Num() > 1)
	{
		StartBattle();
	}
}

void ASweetDreamsBattleManager::InitiateCombatMultiple(TArray<UBattleActorComponent*> InBattlers)
{
	AddMultipleBattlers(InBattlers);

	if (!bIsBattleActive && Battlers.Num() > 1)
	{
		StartBattle();
	}
}

void ASweetDreamsBattleManager::AddBattler(UBattleActorComponent* Battler)
{
	RemoveInvalidBattlers();

	if (IsValid(Battler) && !Battlers.FindByKey(Battler) && Battler->IsAlive())
	{
		BindFunctionsToBattler(Battler);

		Battlers.AddUnique(Battler);
	}
}

void ASweetDreamsBattleManager::AddMultipleBattlers(TArray<UBattleActorComponent*> InBattlers)
{
	for (UBattleActorComponent* Battler : InBattlers)
	{
		AddBattler(Battler);
	}

	RemoveInvalidBattlers();
}

void ASweetDreamsBattleManager::RemoveBattler(UBattleActorComponent* Battler, bool bExitFromCombat)
{
	if (IsValid(Battler) && Battlers.FindByKey(Battler))
	{
		UnbindFunctionsFromBattler(Battler);

		if (bExitFromCombat)
		{
			Battler->SetInCombat(false);
		}

		Battlers.Remove(Battler);
	}
}

void ASweetDreamsBattleManager::RemoveMultipleBattlers(TArray<UBattleActorComponent*> InBattlers, bool bExitFromCombat)
{
	for (UBattleActorComponent* Battler : InBattlers)
	{
		RemoveBattler(Battler, bExitFromCombat);
	}
}

void ASweetDreamsBattleManager::ClearBattlers(bool bExitFromCombat)
{
	LastBattleBattlers = Battlers;

	for (const TWeakObjectPtr<UBattleActorComponent>& WeakBattler : Battlers)
	{
		if (UBattleActorComponent* Battler = WeakBattler.Get())
		{
			UnbindFunctionsFromBattler(Battler);

			if (bExitFromCombat)
			{
				Battler->SetInCombat(false);
			}
		}
	}

	Battlers.Empty();
}

void ASweetDreamsBattleManager::StartBattle()
{
	if (Battlers.IsEmpty())
	{
		return;
	}

	if (!bIsBattleActive) {
		bIsBattleActive = true;
		OnBattleStart();
		OnBattleStarted.Broadcast();
	}
}

void ASweetDreamsBattleManager::EndBattle()
{
	if (bIsBattleActive && !Battlers.IsEmpty()) {
		bIsBattleActive = false;

		for (const TWeakObjectPtr<UBattleActorComponent>& WeakBattler : Battlers)
		{
			if (UBattleActorComponent* Component = WeakBattler.Get())
			{
				UnbindFunctionsFromBattler(Component);

				Component->SetInCombat(false);
			}
		}

		bIsVictorious = EvaluateBattleVictory();
		if (bIsVictorious)
		{
			OnBattleVictory();
		}
		else
		{
			OnBattleDefeat();
		}

		ClearBattlers(true);
		OnBattleEnd();
		OnBattleEnded.Broadcast();
	}
}

void ASweetDreamsBattleManager::EvaluateBattleEnd()
{
	TSet<ETeamType> AliveTeams;

	if (Battlers.IsEmpty())
	{
		return;
	}

	for (const TWeakObjectPtr<UBattleActorComponent>& WeakBattler : Battlers)
	{
		if (UBattleActorComponent* Component = WeakBattler.Get())
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

void ASweetDreamsBattleManager::OnBattlerKilled_Internal(UBattleActorComponent* Battler)
{
	UnbindFunctionsFromBattler(Battler);

	if (!IsValid(Battler) || Battlers.IsEmpty())
	{
		return;
	}

	if (Battlers.FindByKey(Battler))
	{
		Battler->SetInCombat(false);

		EvaluateBattleEnd();
	}

	OnBattlerKilled(Battler);
}

void ASweetDreamsBattleManager::BindFunctionsToBattler(UBattleActorComponent* Battler)
{
	if (IsValid(Battler))
	{
		Battler->OnKilled.AddUniqueDynamic(this, &ASweetDreamsBattleManager::OnBattlerKilled_Internal);

	}
}

void ASweetDreamsBattleManager::UnbindFunctionsFromBattler(UBattleActorComponent* Battler)
{
	if (IsValid(Battler) && Battlers.FindByKey(Battler))
	{
		Battler->OnKilled.RemoveDynamic(this, &ASweetDreamsBattleManager::OnBattlerKilled_Internal);
	}
}

void ASweetDreamsBattleManager::RemoveInvalidBattlers()
{
	Battlers.RemoveAll([](const TWeakObjectPtr<UBattleActorComponent>& Ptr)
	{
		return !Ptr.IsValid();
	});
}

void ASweetDreamsBattleManager::GetBattlers(TArray<UBattleActorComponent*>& OutBattlers, ETeamType TeamFilter) const
{
	OutBattlers.Reset();
	OutBattlers.Reserve(Battlers.Num());

	for (const TWeakObjectPtr<UBattleActorComponent>& WeakBattler : Battlers)
	{
		if (UBattleActorComponent* Battler = WeakBattler.Get())
		{
			bool bIsValidTeam = TeamFilter == ETeamType::None || Battler->GetTeam() == TeamFilter;

			if (bIsValidTeam)
			{
				OutBattlers.Add(Battler);
			}
		}
	}
}

void ASweetDreamsBattleManager::GetAliveBattlers(TArray<UBattleActorComponent*>& OutBattlers, ETeamType TeamFilter) const
{
	OutBattlers.Reset();
	OutBattlers.Reserve(Battlers.Num());

	for (const TWeakObjectPtr<UBattleActorComponent>& WeakBattler : Battlers)
	{
		if (UBattleActorComponent* Battler = WeakBattler.Get())
		{
			bool bIsValidTeam = TeamFilter == ETeamType::None || Battler->GetTeam() == TeamFilter;

			if (Battler->IsAlive() && bIsValidTeam)
			{
				OutBattlers.Add(Battler);
			}
		}
	}
}

void ASweetDreamsBattleManager::GetLastBattleBattlers(TArray<UBattleActorComponent*>& OutBattlers, ETeamType TeamFilter) const
{
	OutBattlers.Reset();
	OutBattlers.Reserve(LastBattleBattlers.Num());

	for (const TWeakObjectPtr<UBattleActorComponent>& WeakBattler : LastBattleBattlers)
	{
		if (UBattleActorComponent* Battler = WeakBattler.Get())
		{
			bool bIsValidTeam = TeamFilter == ETeamType::None || Battler->GetTeam() == TeamFilter;

			if (bIsValidTeam)
			{
				OutBattlers.Add(Battler);
			}
		}
	}
}

