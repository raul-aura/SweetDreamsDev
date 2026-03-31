
#include "BattleEvents/BattleEventDamage.h"
#include "Battle/BattleActorComponent.h"

UBattleEventDamage::UBattleEventDamage()
{
	bOverrideTargetSettings = true;
}

void UBattleEventDamage::ExecuteEvent_Implementation()
{
	if (BattleContext)
	{
		BattleContext->Damage(GetExpectedDamageAmount(), DamageTags, TargetSettings);
	}
}

float UBattleEventDamage::GetExpectedDamageAmount() const
{
	float Value = FixedAmount;

	if (UBattleActorComponent* Instigator = BattleContext->GetInstigator())
	{
		for (const TPair<FGameplayTag, float>& Pair : DamageValueParameters)
		{
			if (FBattleParameter* Parameter = Instigator->Parameters.Find(Pair.Key))
			{
				Value += Parameter->GetParameterValue() * FMath::Max(Pair.Value, 0.f);
			}
		}

		for (const FGameplayTag& DamageTag : DamageTags)
		{
			if (FBattleParameter* Parameter = Instigator->Parameters.Find(DamageTag))
			{
				Value *= Parameter->GetParameterValue();
			}
		}
	}

	return Value;
}
