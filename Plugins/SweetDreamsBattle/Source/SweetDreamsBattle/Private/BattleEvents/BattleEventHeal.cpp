

#include "BattleEvents/BattleEventHeal.h"
#include "Battle/BattleActorComponent.h"

UBattleEventHeal::UBattleEventHeal()
{
	bOverrideTargetSettings = true;
}

void UBattleEventHeal::ExecuteEvent_Implementation()
{
	if (BattleContext)
	{
		BattleContext->Heal(GetExpectedHealAmount(), HealTags, TargetSettings);
	}
}

float UBattleEventHeal::GetExpectedHealAmount_Implementation() const
{
	float Value = FixedAmount;

	if (UBattleActorComponent* Instigator = BattleContext->GetInstigator())
	{
		for (const TPair<FGameplayTag, float>& Pair : HealValueParameters)
		{
			if (FBattleParameter* Parameter = Instigator->Parameters.Find(Pair.Key))
			{
				Value += Parameter->GetParameterValue() * FMath::Max(Pair.Value, 0.f);
			}
		}

		for (const FGameplayTag& HealTag : HealTags)
		{
			if (FBattleParameter* Parameter = Instigator->Parameters.Find(HealTag))
			{
				Value *= Parameter->GetParameterValue();
			}
		}
	}

	return Value;
}
