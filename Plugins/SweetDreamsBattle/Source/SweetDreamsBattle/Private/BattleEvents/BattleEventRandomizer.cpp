

#include "BattleEvents/BattleEventRandomizer.h"
#include "Battle/BattleActorComponent.h"
#include "Data/BattleElement.h"

UBattleEventRandomizer::UBattleEventRandomizer()
{
	bAsynchronousEvent = true;
}

void UBattleEventRandomizer::OnEventCreated_Implementation()
{
	if (BattleContext && BattleContext->GetInstigator())
	{
		BattleContext->GetInstigator()->OnBattleElementExecuted.AddUniqueDynamic(this, &UBattleEventRandomizer::IncrementExecution);
	}
}

bool UBattleEventRandomizer::IsElementRandomizable_Implementation() const
{
	if (IsExecutionForced()) return true;
	if (MaxExecutions > 0 && TimesExecuted > MaxExecutions) return false;

	if (BattleContext)
	{
		if (UBattleActorComponent* Instigator = BattleContext->GetInstigator())
		{
			FBattleParameter Param;
			float Current = 0.f;
			float Max = 0.f;

			if (Instigator->GetHealthParameter(Param, Current, Max))
			{
				return Current >= HealthRangeMin && Current <= HealthRangeMax;
			}
		}
	}

	return false;
}

bool UBattleEventRandomizer::IsExecutionForced_Implementation() const
{
	return MinExecutions > 0 && TimesExecuted < MinExecutions;
}

float UBattleEventRandomizer::GetWeight_Implementation() const
{
	return BaseWeight;
}

void UBattleEventRandomizer::IncrementExecution(UBattleActorComponent* BattleActor, UBattleElement* Element)
{
	if (BattleContext && IsValid(Element) && Element == BattleContext->GetOwnerElement())
	{
		TimesExecuted++;
	}
}


