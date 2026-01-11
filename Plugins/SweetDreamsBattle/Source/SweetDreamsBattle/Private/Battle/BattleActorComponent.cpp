
#include "Battle/BattleActorComponent.h"
#include "Battle/SweetDreamsBattleBPLibrary.h"
#include "Battle/SweetDreamsBattleInterface.h"
#include "Battle/SweetDreamsBattleManager.h"

UBattleActorComponent::UBattleActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UBattleActorComponent* UBattleActorComponent::GetBattleActorComponent(const AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UBattleActorComponent>() : nullptr;
}

ETeamType UBattleActorComponent::GetTeam() const
{
	return Team;
}

void UBattleActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBattleActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBattleActorComponent::Damage(AActor* Target, float Amount)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Amount = FMath::Abs(Amount) * -1;

		DamageDealt += FMath::Abs(Amount);
		Component->ReceiveDamage(GetOwner(), Amount);
		OnDamage.Broadcast(Target, FMath::Abs(Amount));

		if (bStartCombatOnDamage && USweetDreamsBattleBPLibrary::AreTeamsHostile(this->Team, Component->Team))
		{
			// TO DO: call InitiateCombatWith
		}

		for (UObject* Dependent : GetBattleDependents())
		{
			ISweetDreamsBattleInterface::Execute_OnDamage(Dependent, Target, Amount);
		}
	}
}

void UBattleActorComponent::ReceiveDamage(AActor* Instigator, float Amount)
{
	Amount = FMath::Abs(Amount) * -1;
	AddModifierToParameter(Health, EParameterModifierType::Absolute, Amount);

	OnReceiveDamage.Broadcast(Instigator, FMath::Abs(Amount));
}

void UBattleActorComponent::Heal(AActor* Target, float Amount)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Amount = FMath::Abs(Amount);

		HealingDealt += FMath::Abs(Amount);
		Component->ReceiveHeal(GetOwner(), Amount);
		OnHeal.Broadcast(Target, Amount);
	}
}

void UBattleActorComponent::ReceiveHeal(AActor* Instigator, float Amount)
{
	Amount = FMath::Abs(Amount);
	AddModifierToParameter(Health, EParameterModifierType::Absolute, Amount);

	if (UBattleActorComponent* Component = GetBattleActorComponent(Instigator))
	{
		OnReceiveHeal.Broadcast(Instigator, Amount);
	}
}

void UBattleActorComponent::ReceiveKill(AActor* Instigator)
{
	if (IsAlive())
	{
		bIsAlive = false;

		if (UBattleActorComponent* Component = GetBattleActorComponent(Instigator))
		{
			OnReceiveKill.Broadcast(Instigator);
		}
	}
}

void UBattleActorComponent::Kill_Target(AActor* Target)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Component->ReceiveKill(GetOwner());
		OnKill.Broadcast(Target);
	}
}

void UBattleActorComponent::ReceiveRevive(AActor* Instigator)
{
	if (!IsAlive())
	{
		bIsAlive = true;

		if (UBattleActorComponent* Component = GetBattleActorComponent(Instigator))
		{
			OnReceiveRevive.Broadcast(Instigator);
		}
	}
}

void UBattleActorComponent::Revive_Target(AActor* Target)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Component->ReceiveRevive(GetOwner());
		OnRevive.Broadcast(Target);
	}
}

void UBattleActorComponent::SetInCombat(bool bInIsInCombat)
{
	if (bIsInCombat == bInIsInCombat) return;

	bIsInCombat = bInIsInCombat;
	if (bIsInCombat) 
	{
		OnEnterCombat.Broadcast();
	}
	else
	{
		OnExitCombat.Broadcast();
	}
}

bool UBattleActorComponent::IsAlive() const
{
	return bIsAlive;
}

bool UBattleActorComponent::IsInCombat() const
{
	return bIsInCombat;
}

void UBattleActorComponent::AddModifierToParameter(UPARAM(ref) FBattleParamater& Parameter, EParameterModifierType ModifierType, float ModifierValue)
{
	FBattleParameterModifier Modifier(ModifierType, ModifierValue);
	Parameter.AddModifier(Modifier);
	OnParameterChange.Broadcast(Parameter, Modifier);
}

void UBattleActorComponent::RemoveModifierFromParameter(UPARAM(ref) FBattleParamater& Parameter, FBattleParameterModifier Modifier)
{
	OnParameterChange.Broadcast(Parameter, Modifier);
	Parameter.RemoveModifier(Modifier);
}

void UBattleActorComponent::StartBattle()
{
	if (TObjectPtr<ASweetDreamsBattleManager> BattleManager = ASweetDreamsBattleManager::GetBattleManager(GetOwner()))
	{
		BattleManager->StartBattle(GetOwner());
	}
}

TArray<UObject*> UBattleActorComponent::GetBattleDependents() const
{
	TArray<UObject*> Dependents;

	if (GetOwner()->Implements<USweetDreamsBattleInterface>())
	{
		Dependents.Add(GetOwner());
	}

	// TODO:: get uobject and other dependencies with interface and add to array

	return Dependents;
}


