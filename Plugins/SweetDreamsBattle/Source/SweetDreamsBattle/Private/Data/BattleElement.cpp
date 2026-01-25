
#include "Data/BattleElement.h"
#include "Battle/BattleActorComponent.h"

UBattleElement* UBattleElement::CreateBattleElement(const UObject* WorldContext, TSubclassOf<UBattleElementData> DataClass)
{
	const UBattleActorComponent* BattleActor = Cast<UBattleActorComponent>(WorldContext);

	if (IsValid(BattleActor))
	{
		//add to battle component array
	}
	else
	{
		//print not found component to add battle element and to tick.
	}

	return nullptr;
}

void UBattleElement::Tick(float DeltaTime)
{
	//if (Index >= Events.Num()) return;

	//if (!CurrentEvent)
	//{
	//	CurrentEvent = Events[Index];
	//	CurrentEvent->Start(Context);
	//	return;
	//}

	//if (CurrentEvent->IsFinished())
	//{
	//	CurrentEvent = nullptr;
	//	Index++;
	//}
}

void UBattleElement::End()
{
	//broadcast that finished element and may be destroyed
}
