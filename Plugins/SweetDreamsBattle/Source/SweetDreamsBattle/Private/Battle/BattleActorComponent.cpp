
#include "Battle/BattleActorComponent.h"
#include "Battle/SweetDreamsBattleBPLibrary.h"
#include "Battle/SweetDreamsBattleInterface.h"

UBattleActorComponent::UBattleActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UBattleActorComponent* UBattleActorComponent::GetBattleActorComponent(const AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UBattleActorComponent>() : nullptr;
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
		Component->ReceiveDamage(GetOwner(), Amount);
		OnDamageDelegate.Broadcast(this, Component, Amount);

		if (USweetDreamsBattleBPLibrary::AreTeamsHostile(this->Team, Component->Team))
		{
			SetInCombat(true);
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

	if (UBattleActorComponent* Component = GetBattleActorComponent(Instigator))
	{
		OnReceiveDamageDelegate.Broadcast(Component, this, Amount);

		if (USweetDreamsBattleBPLibrary::AreTeamsHostile(this->Team, Component->Team))
		{
			SetInCombat(true);
		}
	}
}

void UBattleActorComponent::Heal(AActor* Target, float Amount)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Amount = FMath::Abs(Amount);
		Component->ReceiveHeal(GetOwner(), Amount);
		OnHealDelegate.Broadcast(this, Component, Amount);
	}
}

void UBattleActorComponent::ReceiveHeal(AActor* Instigator, float Amount)
{
	Amount = FMath::Abs(Amount);
	AddModifierToParameter(Health, EParameterModifierType::Absolute, Amount);

	if (UBattleActorComponent* Component = GetBattleActorComponent(Instigator))
	{
		OnReceiveHealDelegate.Broadcast(Component, this, Amount);
	}
}

void UBattleActorComponent::Kill(AActor* Instigator)
{
	if (IsAlive())
	{
		bIsAlive = false;

		if (UBattleActorComponent* Component = GetBattleActorComponent(Instigator))
		{
			OnReceiveKillDelegate.Broadcast(Component, this);
		}
	}
}

void UBattleActorComponent::Kill_Target(AActor* Target)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Component->Kill(GetOwner());
		OnKillDelegate.Broadcast(this, Component);
	}
}

void UBattleActorComponent::Revive(AActor* Instigator)
{
	if (!IsAlive())
	{
		bIsAlive = true;

		if (UBattleActorComponent* Component = GetBattleActorComponent(Instigator))
		{
			OnReceiveReviveDelegate.Broadcast(Component, this);
		}
	}
}

void UBattleActorComponent::Revive_Target(AActor* Target)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Component->Revive(GetOwner());
		OnReviveDelegate.Broadcast(this, Component);
	}
}

void UBattleActorComponent::SetInCombat(bool bInIsInCombat)
{
	if (bIsInCombat == bInIsInCombat) return;

	bIsInCombat = bInIsInCombat;
	if (bIsInCombat) 
	{
		OnEnterCombatDelegate.Broadcast(this);
	}
	else
	{
		OnExitCombatDelegate.Broadcast(this);
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
	OnParameterChangeDelegate.Broadcast(Parameter, Modifier);
}

void UBattleActorComponent::RemoveModifierFromParameter(UPARAM(ref) FBattleParamater& Parameter, FBattleParameterModifier Modifier)
{
	OnParameterChangeDelegate.Broadcast(Parameter, Modifier);
	Parameter.RemoveModifier(Modifier);
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


