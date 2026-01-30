
#include "Data/BattleElement.h"
#include "Data/BattleElementData.h"
#include "Battle/BattleActorComponent.h"
#include "BattleElement.h"

UBattleElement* UBattleElement::CreateBattleElement(UBattleActorComponent* BattleComponent, UBattleElementData* Data, TSubclassOf<UBattleElement> CustomClass)
{
	if (IsValid(BattleComponent))
	{
		UBattleElement* Element = NewObject<UBattleElement>(BattleComponent, CustomClass, Data->ElementUniqueName, RF_Transient);

		if (IsValid(Element))
		{
			// TODO: populate this class with variables from data
			Element->BattleElementData = Data;
			Element->Phases = Data->Phases; //check if it needs to duplicate
			Element->Owner = BattleComponent;

			BattleComponent->RegisterBattleElement(Element);

			return Element;
		}
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
	OnBattleElementEnd.ExecuteIfBound(this);
}

void UBattleElement::DuplicateEvents()
{
	FBattleElementEventPhase& RuntimePhase = Element->Phases.AddDefaulted_GetRef();

	// Copy phase-level data
	RuntimePhase.bSkipEventsCompletion = PhaseTemplate.bSkipEventsCompletion;

	// Duplicate events
	RuntimePhase.Events.Reserve(PhaseTemplate.Events.Num());
	for (UBattleEvent* EventTemplate : PhaseTemplate.Events)
	{
		if (!EventTemplate) continue;

		UBattleEvent* RuntimeEvent =
			DuplicateObject<UBattleEvent>(EventTemplate, Element);

		RuntimePhase.Events.Add(RuntimeEvent);
	}
}
