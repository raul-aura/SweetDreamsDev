
#include "Data/BattleElement.h"
#include "Data/BattleElementData.h"
#include "Data/BattleEvent.h"
#include "Data/BattleContext.h"
#include "Battle/BattleActorComponent.h"

UBattleElement* UBattleElement::CreateBattleElement(UBattleActorComponent* BattleComponent, 
	UBattleElementData* Data, const TArray<UBattleActorComponent*>& Targets,
	TSubclassOf<UBattleElement> CustomClass, TSubclassOf<UBattleContext> CustomContextClass, 
	bool bShouldUnregisterOnEnd, bool bAutoExecute)
{
	if (IsValid(BattleComponent) && IsValid(Data))
	{
		TSubclassOf<UBattleElement> ElementClass = CustomClass.Get() ? CustomClass.Get() : UBattleElement::StaticClass();
		TSubclassOf<UBattleContext> ContextClass = CustomContextClass.Get() ? CustomContextClass.Get() : UBattleContext::StaticClass();

		UBattleElement* Element = NewObject<UBattleElement>(BattleComponent, ElementClass, Data->ElementUniqueName, RF_Transient);

		if (IsValid(Element))
		{
			Element->BattleElementData = Data;
			Element->Owner = BattleComponent;
			Element->bUnregisterOnEnd = bShouldUnregisterOnEnd;

			Element->CreateBattleContext(Targets, ContextClass);
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

void UBattleElement::Execute(bool bResetExecution)
{
	if (Events.IsEmpty())
	{
		End();
	}

	bElementInExecution = true;

	if (bResetExecution)
	{
		CurrentEvent = nullptr;
		CurrentEventIndex = 0;
	}

	OnBattleElementExecuted.Broadcast();
}

void UBattleElement::Tick(float DeltaTime)
{
	CachedDeltaTime = DeltaTime;

	if (bElementInExecution)
	{
		EvaluateEvents(DeltaTime);
		EvaluateAsyncEvents(DeltaTime);
	}
}

void UBattleElement::End()
{
	bElementInExecution = false;
	CurrentEvent = nullptr;
	CurrentEventIndex = 0;
	ActiveAsyncEvents.Empty();

	OnBattleElementEnded.Broadcast();
	OnBattleElementEnd.ExecuteIfBound(this);
}

void UBattleElement::EndBattleEvents()
{
	for (UBattleEvent* Event : Events)
	{
		if (IsValid(Event) && !Event->IsFinished())
		{
			Event->EndEvent();
		}
	}
}

void UBattleElement::UpdateContextCandidates(TArray<UBattleActorComponent*> InCandidates)
{
	if (BattleContext)
	{
		BattleContext->UpdateCandidates(InCandidates);
	}
}

TArray<UBattleEvent*> UBattleElement::GetBattleEvents() const
{
	return Events;
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

	for (const UBattleEvent* Event : BattleElementData->Events)
	{
		if (IsValid(Event))
		{
			UBattleEvent* RuntimeEvent = DuplicateObject<UBattleEvent>(Event, this);

			if (IsValid(RuntimeEvent))
			{
				RuntimeEvent->BattleContext = BattleContext;
				Events.Add(RuntimeEvent);

				RuntimeEvent->OnEventCreated();
			}
		}
	}
}

void UBattleElement::EvaluateEvents(float DeltaTime)
{
	if (CurrentEventIndex >= Events.Num()) return;

	if (!IsValid(CurrentEvent))
	{
		StartCurrentEvent();

		if (!IsValid(CurrentEvent)) return;
	}

	CurrentEvent->Tick(DeltaTime);

	if (CurrentEvent->IsFinished())
	{
		CurrentEvent->EndEvent();
		AdvanceEvent();
	}
}

void UBattleElement::EvaluateAsyncEvents(float DeltaTime)
{
	for (int32 i = ActiveAsyncEvents.Num() - 1; i >= 0; --i)
	{
		UBattleEvent* Event = ActiveAsyncEvents[i];

		if (!Event)
		{
			ActiveAsyncEvents.RemoveAtSwap(i);
			continue;
		}

		const bool bFinished = Event->IsFinished();

		if (bFinished)
		{
			Event->EndEvent();
			ActiveAsyncEvents.RemoveAtSwap(i);
			continue;
		}

		Event->Tick(DeltaTime);
	}

	if (ActiveAsyncEvents.Num() == 0 && CurrentEventIndex >= Events.Num())
	{
		End();
	}
}

void UBattleElement::StartCurrentEvent()
{
	if (!Events.IsValidIndex(CurrentEventIndex)) return;

	UBattleEvent* Event = Events[CurrentEventIndex];
	CurrentEvent = Event;

	if (!Event)
	{
		AdvanceEvent();
		return;
	}

	const bool bIsAsync = Event->bAsynchronousEvent;

	Event->ExecuteEvent();

	if (bIsAsync)
	{
		ActiveAsyncEvents.Add(Event);
		AdvanceEvent();
	}
}

void UBattleElement::AdvanceEvent()
{
	CurrentEvent = nullptr;
	CurrentEventIndex++;

	if (CurrentEventIndex >= Events.Num())
	{
		if (ActiveAsyncEvents.Num() == 0)
		{
			End();
		}
	}
}
