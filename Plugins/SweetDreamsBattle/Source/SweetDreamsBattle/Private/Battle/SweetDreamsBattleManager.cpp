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
		BindFunctionsToActor(Battler);

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

void ASweetDreamsBattleManager::InitiateCombatBetween(AActor* Actor1, AActor* Actor2)
{
	if (bIsBattleActive)
	{
		AddActorToBattle(Actor1);
		AddActorToBattle(Actor2);
		return;
	}

	AddActorToBattle(Actor1);
	AddActorToBattle(Actor2);

	TArray<AActor*> ValidBattlers = GetBattlers();
	if (ValidBattlers.Num() >= 2 && !bIsBattleActive)
	{
		StartBattle();
	}
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
	}
}

void ASweetDreamsBattleManager::EndBattle()
{
	if (bIsBattleActive && !Battlers.IsEmpty()) {
		bIsBattleActive = false;

		for (const TWeakObjectPtr<AActor>& WeakBattler : Battlers)
		{
			if (AActor* Battler = WeakBattler.Get())
			{
				UnbindFunctionsFromActor(Battler);

				if (UBattleActorComponent* Component = UBattleActorComponent::GetBattleActorComponent(Battler))
				{
					if (Component->IsAlive())
					{
						Component->SetInCombat(false);
					}
				}
			}
		}

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

void ASweetDreamsBattleManager::ClearBattlers(bool bExitFromCombat)
{
	if (bExitFromCombat)
	{
		for (const TWeakObjectPtr<AActor>& WeakBattler : Battlers)
		{
			if (AActor* Battler = WeakBattler.Get())
			{
				if (UBattleActorComponent* Component = UBattleActorComponent::GetBattleActorComponent(Battler))
				{
					if (Component->IsAlive())
					{
						Component->SetInCombat(false);
					}
				}
			}
		}
	}

	Battlers.Empty();
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

	if (Battlers.FindByKey(Target))
	{
		UnbindFunctionsFromActor(Target);

		if (UBattleActorComponent* Component = UBattleActorComponent::GetBattleActorComponent(Target))
		{
			Component->SetInCombat(false);
		}

		EvaluateBattleEnd();
	}
}

void ASweetDreamsBattleManager::BindFunctionsToActor(AActor* Battler)
{
	if (IsValid(Battler))
	{
		if (UBattleActorComponent* Component = UBattleActorComponent::GetBattleActorComponent(Battler))
		{
			//Component->OnKill.AddUniqueDynamic(this, &ASweetDreamsBattleManager::OnBattlerKilled);
		}
	}
}

void ASweetDreamsBattleManager::UnbindFunctionsFromActor(AActor* Battler)
{
	if (IsValid(Battler) && Battlers.FindByKey(Battler))
	{
		if (UBattleActorComponent* Component = UBattleActorComponent::GetBattleActorComponent(Battler))
		{
			//Component->OnKill.RemoveDynamic(this, &ASweetDreamsBattleManager::OnBattlerKilled);
		}
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
	OutBattlers.Reserve(Battlers.Num());

	for (const TWeakObjectPtr<AActor>& WeakBattler : Battlers)
	{
		if (AActor* Battler = WeakBattler.Get())
		{
			OutBattlers.Add(Battler);
		}
	}

	return OutBattlers;
}

TArray<AActor*> ASweetDreamsBattleManager::GetAliveBattlers() const
{
	TArray<AActor*> OutBattlers;
	OutBattlers.Reserve(Battlers.Num());

	for (const TWeakObjectPtr<AActor>& WeakBattler : Battlers)
	{
		if (AActor* Battler = WeakBattler.Get())
		{
			if (UBattleActorComponent* Component = UBattleActorComponent::GetBattleActorComponent(Battler))
			{
				if (Component->IsAlive())
				{
					OutBattlers.Add(Battler);
				}
			}
		}
	}

	return OutBattlers;
}

