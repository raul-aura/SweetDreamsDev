
#include "Data/BattleContext.h"
#include "Data/BattleElement.h"
#include "Battle/BattleActorComponent.h"

void UBattleContext::Initialize(UBattleElement* Owner, TObjectPtr<UBattleActorComponent> InInstigator, TArray<UBattleActorComponent*> InTargets)
{
	Instigator = InInstigator;
	Targets = InTargets;
	OwnerElement = Owner;
}

UBattleElement* UBattleContext::GetOwnerElement() const
{
	return OwnerElement.Get();
}

void UBattleContext::Damage(const FBattleParamater& Value, float FlatValue)
{
	const float Damage = Value.CurrentValue + FMath::Abs(FlatValue);

	for (TObjectPtr<UBattleActorComponent> Target : Targets)
	{
		Target->Damage(Damage);
	}

	OnDamageDealt.Broadcast(FBattleContextWrapper(Instigator, Targets, Damage));
}

void UBattleContext::Heal(const FBattleParamater& Value, float FlatValue)
{
	const float Heal = Value.CurrentValue + FMath::Abs(FlatValue);

	for (TObjectPtr<UBattleActorComponent> Target : Targets)
	{
		Target->Heal(Heal);
	}

	OnHealingDealt.Broadcast(FBattleContextWrapper(Instigator, Targets, Heal));
}

void UBattleContext::Kill()
{
	for (TObjectPtr<UBattleActorComponent> Target : Targets)
	{
		Target->SetIsAlive(false);
	}

	OnKill.Broadcast(FBattleContextWrapper(Instigator, Targets, 0.f));
}

void UBattleContext::Ressurect()
{
	for (TObjectPtr<UBattleActorComponent> Target : Targets)
	{
		Target->SetIsAlive(true);
	}

	OnRessurect.Broadcast(FBattleContextWrapper(Instigator, Targets, 0.f));
}

void UBattleContext::RequestBattleElementEnd()
{
	if (UBattleElement* Element = GetOwner())
	{
		Element->End();
	}
}

float UBattleContext::GetDeltaTime() const
{
	if (const UBattleElement* Element = GetOwner())
	{
		return Element->CachedDeltaTime;
	}

	return 0.f;
}
