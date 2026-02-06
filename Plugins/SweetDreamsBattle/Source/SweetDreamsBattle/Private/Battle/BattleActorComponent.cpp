
#include "Battle/BattleActorComponent.h"
#include "Battle/SweetDreamsBattleBPLibrary.h"
#include "Battle/SweetDreamsBattleInterface.h"
#include "Battle/SweetDreamsBattleManager.h"
#include "Data/BattleElement.h"

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

	Health.Initialize();
	Strength.EvaluateModifiers();
	Resistence.EvaluateModifiers();

	for (TPair<FName, FBattleParamater>& Pair : CustomParameters)
	{
		Pair.Value.EvaluateModifiers();
	}
}

void UBattleActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UBattleActorComponent::RegisterBattleElement(UBattleElement* BattleElement)
{
	if (IsValid(BattleElement))
	{
		BattleElements.AddUnique(BattleElement);
		BattleElement->OnBattleElementEnd.BindUObject(this, &UBattleActorComponent::UnregisterBattleElement);

		return true;
	}

	return false;
}

void UBattleActorComponent::UnregisterBattleElement(UBattleElement* BattleElement)
{
	if (IsValid(BattleElement))
	{
		if (BattleElements.Contains(BattleElement))
		{
			BattleElements.Remove(BattleElement);
		}
	}
}

void UBattleActorComponent::Damage(float Value)
{
	Health.Damage(Value);

	if (GetHealth().CurrentHealth <= 0)
	{
		SetIsAlive(false);
	}
}

void UBattleActorComponent::Heal(float Value)
{
	if (!IsAlive()) return;

	Health.Heal(Value);
}

void UBattleActorComponent::AddModifierToMaxHealth(EParameterModifierType ModifierType, float ModifierValue)
{
	FBattleParameterModifier Modifier(ModifierType, ModifierValue);

	Health.MaxHealth.AddModifier(Modifier);
	Health.OnMaxHealthChanged(true);

	OnParameterChange.Broadcast(Health.MaxHealth, Modifier);

}

void UBattleActorComponent::RemoveModifierFromMaxHealth(FBattleParameterModifier Modifier)
{
	Health.MaxHealth.RemoveModifier(Modifier);
	Health.OnMaxHealthChanged(true);

	OnParameterChange.Broadcast(Health.MaxHealth, Modifier);
}

void UBattleActorComponent::SetIsAlive(bool bInIsAlive)
{
	if (bIsAlive == bInIsAlive) return;

	bIsAlive = bInIsAlive;
	if (bIsAlive)
	{
		OnRessurected.Broadcast();
	}
	else
	{
		OnKilled.Broadcast();
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

ETeamType UBattleActorComponent::GetTeam() const
{
	return Team;
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

const FHealthParameter& UBattleActorComponent::GetHealth() const
{
	return Health;
}

const FBattleParamater& UBattleActorComponent::GetStrength() const
{
	return Strength;
}

const FBattleParamater& UBattleActorComponent::GetResistence() const
{
	return Resistence;
}

bool UBattleActorComponent::GetCustomParameter(FName Parameter, FBattleParamater& OutParam) const
{
	if (const FBattleParamater* Param = CustomParameters.Find(Parameter))
	{
		OutParam = *Param;
		return true;
	}

	return false;
}

float UBattleActorComponent::GetCustomSimpleParameter(FName Parameter, float Percentage) const
{
	if (const float* Param = CustomSimpleParameters.Find(Parameter))
	{
		return *Param * Percentage;
	}

	return 0.f;
}

void UBattleActorComponent::InitiateCombat(AActor* OtherActor, bool bCheckForHostility)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	if (bCheckForHostility)
	{
		if (UBattleActorComponent* OtherComponent = GetBattleActorComponent(OtherActor))
		{
			if (!USweetDreamsBattleBPLibrary::AreTeamsHostile(this->Team, OtherComponent->Team))
			{
				return;
			}
		}
	}

	if (ASweetDreamsBattleManager* BattleManager = ASweetDreamsBattleManager::GetBattleManager(GetOwner()))
	{
		BattleManager->InitiateCombatBetween(GetOwner(), OtherActor);
	}
}

TArray<UObject*> UBattleActorComponent::GetBattleDependents() const
{
	TArray<UObject*> Dependents;

	// TODO:: get uobject and other dependencies with interface and add to array

	return Dependents;
}


