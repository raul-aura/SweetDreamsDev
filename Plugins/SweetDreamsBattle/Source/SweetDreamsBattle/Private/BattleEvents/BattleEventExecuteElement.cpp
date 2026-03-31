

#include "BattleEvents/BattleEventExecuteElement.h"
#include "Battle/BattleActorComponent.h"

UBattleEventExecuteElement::UBattleEventExecuteElement()
{
	bOverrideTargetSettings = true;
}

void UBattleEventExecuteElement::ExecuteEvent_Implementation()
{
	if (BattleContext && BattleElementData)
	{
		const TArray<UBattleActorComponent*> Targets = BattleContext->GetSelectedTargets(TargetSettings);

		for (UBattleActorComponent* Target : Targets)
		{
			UBattleElement* OutElement = nullptr;
			Target->ExecuteBattleElement(BattleElementData, OutElement);
		}
	}
}


