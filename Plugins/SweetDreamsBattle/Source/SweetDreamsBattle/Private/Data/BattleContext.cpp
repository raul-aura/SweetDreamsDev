
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

void UBattleContext::Damage(float Value)
{
	Value = FMath::Abs(Value);

	for (TObjectPtr<UBattleActorComponent> Target : Targets)
	{
		Target->Damage(Value);
	}
}

void UBattleContext::Heal(float Value)
{
	Value = FMath::Abs(Value);

	for (TObjectPtr<UBattleActorComponent> Target : Targets)
	{
		Target->Heal(Value);
	}
}

void UBattleContext::Kill()
{
	for (TObjectPtr<UBattleActorComponent> Target : Targets)
	{
		Target->SetIsAlive(false);
	}
}

void UBattleContext::Ressurect()
{
	for (TObjectPtr<UBattleActorComponent> Target : Targets)
	{
		Target->SetIsAlive(true);
	}
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
