
#include "Data/BattleElement.h"
#include "Data/BattleElementData.h"
#include "Data/BattleEvent.h"
#include "Data/BattleContext.h"
#include "Battle/BattleActorComponent.h"

UBattleElement* UBattleElement::CreateBattleElement(UBattleActorComponent* BattleComponent, TArray<UBattleActorComponent*> Targets, UBattleElementData* Data, TSubclassOf<UBattleElement> CustomClass, TSubclassOf<UBattleContext> CustomContextClass, bool bAutoExecute)
{
	if (IsValid(BattleComponent) && IsValid(Data))
	{
		UBattleElement* Element = NewObject<UBattleElement>(BattleComponent, CustomClass, Data->ElementUniqueName, RF_Transient);

		if (IsValid(Element))
		{
			Element->BattleElementData = Data;
			Element->Owner = BattleComponent;

			Element->CreateBattleContext(Targets, CustomContextClass);
			Element->DuplicateEvents();

			BattleComponent->RegisterBattleElement(Element);

			if (bAutoExecute)
			{
				Element->Execute();
			}

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
	bElementInExecution = true;
}

void UBattleElement::Tick(float DeltaTime)
{
	CachedDeltaTime = DeltaTime;

	if (bElementInExecution)
	{
		EvaluateEvents(DeltaTime);
	}
}

void UBattleElement::End()
{
	bElementInExecution = false;

	OnBattleElementEnd.ExecuteIfBound(this);
}

void UBattleElement::CreateBattleContext(TArray<UBattleActorComponent*> InTargets, TSubclassOf<UBattleContext> CustomContextClass)
{
	BattleContext = NewObject<UBattleContext>(this, CustomContextClass);
	BattleContext->Initialize(this, Owner, InTargets);
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
