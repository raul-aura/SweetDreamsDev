
#include "BattleEvents/BattleEventCreateElement.h"
#include "Data/BattleElement.h"
#include "Battle/BattleActorComponent.h"

void UBattleEventCreateElement::ExecuteEvent_Implementation()
{
	if (BattleContext && BattleElementData)
	{
		const TArray<UBattleActorComponent*> Targets = BattleContext->GetSelectedTargets(TargetSettings);

		for (UBattleActorComponent* Target : Targets)
		{
			UBattleElement* Element = UBattleElement::CreateBattleElement(Target, BattleElementData, GetElementTargets(), CustomClass, CustomContextClass, bShouldUnregisterOnEnd, bAutoExecute);
			
			if (IsValid(Element))
			{
				OnBattleElementCreated(Element);
			}
		}
	}
}
