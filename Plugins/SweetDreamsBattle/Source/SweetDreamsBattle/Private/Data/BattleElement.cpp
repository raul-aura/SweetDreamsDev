
#include "Data/BattleElement.h"
#include "Data/BattleElementData.h"
#include "Data/BattleEvent.h"
#include "Battle/BattleActorComponent.h"

#include "Algo/RandomShuffle.h"

UBattleElement* UBattleElement::CreateBattleElement(UBattleActorComponent* InOwner, UBattleElementData* Data, TSubclassOf<UBattleElement> CustomClass)
{
	if (IsValid(InOwner) && IsValid(Data))
	{
		TSubclassOf<UBattleElement> ElementClass = CustomClass.Get() ? CustomClass.Get() : UBattleElement::StaticClass();

		UBattleElement* Element = NewObject<UBattleElement>(InOwner, ElementClass, Data->ElementUniqueName, RF_Transient);

		if (IsValid(Element))
		{
			if (Element->InitializeBattleElement(InOwner, Data))
			{
				return Element;
			}
		}
	}
	else
	{
		//print not found component to add battle element and to tick.
	}

	return nullptr;
}

bool UBattleElement::InitializeBattleElement(UBattleActorComponent* InOwner, UBattleElementData* Data)
{
	if (IsValid(InOwner) && IsValid(Data))
	{
		BattleElementData = Data;
		Owner = InOwner;
		bUnregisterOnEnd = Data->bShouldUnregisterOnEnd;
		bAutoEnd = Data->bAutoEndOnEventsComplete;

		DuplicateEvents();

		Owner->RegisterBattleElement(this);

		if (BattleElementData->bAutoExecute)
		{
			Execute();
		}

		return true;
	}

	return false;
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
	}
}

void UBattleElement::End()
{
	EndBattleEvents();

	bAllEventsCompleted = false;
	bElementInExecution = false;
	CurrentEvent = nullptr;
	CurrentEventIndex = 0;

	OnBattleElementEnded.Broadcast();
	OnBattleElementEnd.ExecuteIfBound(this);
}

void UBattleElement::EndBattleEvents()
{
	for (UBattleEvent* Event : Events)
	{
		if (IsValid(Event))
		{
			Event->EndEvent();
		}
	}
}

TArray<UBattleActorComponent*> UBattleElement::GetSelectedTargets(const FSelectedTargetsSettings& Settings) const
{
	TArray<UBattleActorComponent*> SelectedTargets;
	TArray<UBattleActorComponent*> CachedTargets = CandidateBattleActors;

	switch (Settings.TargetType)
	{
	case ETargetSelectionScope::Candidates:
		break;
	case ETargetSelectionScope::CandidatesAndOwner:
		CachedTargets.Add(Owner);
		break;
	case ETargetSelectionScope::OwnerOnly:
		SelectedTargets.Add(Owner);
		return SelectedTargets;
	default:
		break;
	}

	if (Settings.bRandomizeSelection)
	{
		Algo::RandomShuffle(CachedTargets);
	}

	if (Settings.MaxAmount > 0)
	{
		const int32 Count = FMath::Min(Settings.MaxAmount, CachedTargets.Num());
		SelectedTargets.Append(CachedTargets.GetData(), Count);
	}
	else
	{
		SelectedTargets = CachedTargets;
	}

	return SelectedTargets;
}

UBattleActorComponent* UBattleElement::GetOwner() const
{
	return Owner;
}

TArray<UBattleEvent*> UBattleElement::GetBattleEvents() const
{
	return Events;
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
				Events.Add(RuntimeEvent);

				RuntimeEvent->InitializeBattleEvent(this);
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
		AdvanceEvent();
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

	Event->ExecuteEvent();
}

void UBattleElement::AdvanceEvent()
{
	CurrentEvent = nullptr;
	CurrentEventIndex++;

	if (CurrentEventIndex >= Events.Num())
	{
		HandleEventsComplete();
	}
}

void UBattleElement::HandleEventsComplete()
{
	if (bAllEventsCompleted)
	{
		return;
	}

	bAllEventsCompleted = true;

	if (bAutoEnd)
	{
		End();
	}
}
