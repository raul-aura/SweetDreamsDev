
#include "Data/BattleElement.h"
#include "Data/BattleElementData.h"
#include "Data/BattleEvent.h"
#include "Data/BattleContext.h"
#include "Battle/BattleActorComponent.h"

UBattleElement* UBattleElement::CreateBattleElement(UBattleActorComponent* BattleComponent, TArray<UBattleActorComponent*> Targets, UBattleElementData* Data, TSubclassOf<UBattleElement> CustomClass)
{
	if (IsValid(BattleComponent) && IsValid(Data))
	{
		UBattleElement* Element = NewObject<UBattleElement>(BattleComponent, CustomClass, Data->ElementUniqueName, RF_Transient);

		if (IsValid(Element))
		{
			Element->BattleElementData = Data;
			Element->Owner = BattleComponent;

			BattleComponent->RegisterBattleElement(Element);

			Element->CreateBattleContext(Targets);
			Element->DuplicateEvents();

			Element->Execute();

			return Element;
		}
	}
	else
	{
		//print not found component to add battle element and to tick.
	}

	return nullptr;
}

void UBattleElement::Execute()
{
	// TODO: get owner and targets and create battle context
	// enable tick
}

void UBattleElement::Tick(float DeltaTime)
{
	EvaluateEvents(DeltaTime);
}

void UBattleElement::End()
{
	OnBattleElementEnd.ExecuteIfBound(this);
}

void UBattleElement::CreateBattleContext(TArray<UBattleActorComponent*> InTargets)
{
	BattleContext = NewObject<UBattleContext>(this);
	BattleContext->Instigator = Owner;
	BattleContext->Targets = InTargets;
	BattleContext->OwnerElement = this;
}

void UBattleElement::DuplicateEvents()
{
	Events.Reset();
	Events.Reserve(BattleElementData->Events.Num());

	for (const TObjectPtr<UBattleEvent> Event : BattleElementData->Events)
	{
		TObjectPtr<UBattleEvent> RuntimeEvent = DuplicateObject<UBattleEvent>(Event, this);
		RuntimeEvent->BattleContext = BattleContext;
		Events.Add(RuntimeEvent);
	}
}

void UBattleElement::EvaluateEvents(float DeltaTime)
{
	if (CurrentEventIndex < Events.Num())
	{
		if (!IsValid(CurrentEvent))
		{
			StartCurrentEvent();
		}
		else if (IsValid(CurrentEvent) && CurrentEvent->IsFinished())
		{
			AdvanceEvent();
		}
	}
}

void UBattleElement::StartCurrentEvent()
{
	if (!Events.IsValidIndex(CurrentEventIndex)) return;

	CurrentEvent = Events[CurrentEventIndex];
	CurrentEvent->Start();
}

void UBattleElement::AdvanceEvent()
{
	CurrentEvent = nullptr;
	CurrentEventIndex++;

	if (CurrentEventIndex >= Events.Num() && EndMode == EBattleElementEndMode::Auto)
	{
		End();
	}
}
