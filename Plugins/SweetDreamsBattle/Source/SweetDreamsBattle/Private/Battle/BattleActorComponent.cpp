
#include "Battle/BattleActorComponent.h"
#include "Battle/SweetDreamsBattleBPLibrary.h"
#include "Battle/SweetDreamsBattleInterface.h"

UBattleActorComponent::UBattleActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UBattleActorComponent* UBattleActorComponent::GetBattleActorComponent(const AActor* Actor)
{
	UBattleActorComponent* Component = Actor->FindComponentByClass<UBattleActorComponent>();
	return Component;
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
	}
}

void UBattleActorComponent::ReceiveHeal(AActor* Instigator, float Amount)
{
	Amount = FMath::Abs(Amount);
	AddModifierToParameter(Health, EParameterModifierType::Absolute, Amount);
}

void UBattleActorComponent::Kill()
{
	if (IsAlive())
	{
		bIsAlive = false;
	}
}

void UBattleActorComponent::Kill_Target(AActor* Target)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Component->Kill();
	}
}

void UBattleActorComponent::Ressurect()
{
	if (!IsAlive())
	{
		bIsAlive = true;
	}
}

void UBattleActorComponent::Ressurect_Target(AActor* Target)
{
	if (UBattleActorComponent* Component = GetBattleActorComponent(Target))
	{
		Component->Ressurect();
	}
}

void UBattleActorComponent::SetInCombat(bool bInIsInCombat)
{
	if (bIsInCombat == bInIsInCombat) return;

	bIsInCombat = bInIsInCombat;
	if (bIsInCombat) 
	{
		// TODO: start battle
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
}

void UBattleActorComponent::RemoveModifierFromParameter(UPARAM(ref) FBattleParamater& Parameter, FBattleParameterModifier Modifier)
{
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


