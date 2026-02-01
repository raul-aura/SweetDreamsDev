
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
	Value = FMath::Abs(Value) * -1;

	for (TObjectPtr<UBattleActorComponent> Target : Targets)
	{
		Target->ModifyHealth(Value);
	}
}

void UBattleContext::Heal(float Value)
{

}

void UBattleContext::Kill()
{

}

void UBattleContext::Ressurect()
{

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
