
#include "Data/BattleElement.h"
#include "Data/BattleElementData.h"
#include "Data/BattleEvent.h"
#include "Battle/BattleActorComponent.h"

UBattleElement* UBattleElement::CreateBattleElement(UBattleActorComponent* BattleComponent, UBattleElementData* Data, TSubclassOf<UBattleElement> CustomClass)
{
	if (IsValid(BattleComponent))
	{
		UBattleElement* Element = NewObject<UBattleElement>(BattleComponent, CustomClass, Data->ElementUniqueName, RF_Transient);

		if (IsValid(Element))
		{
			Element->BattleElementData = Data;
			Element->DuplicateEvents();
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
	EvaluatePhases(DeltaTime);

	//TO DO: if current event from current phase finished, AdvanceCurrentPhaseEvent()
}

void UBattleElement::End()
{
	OnBattleElementEnd.ExecuteIfBound(this);
}

void UBattleElement::DuplicateEvents()
{
	Phases.Reset();
	Phases.Reserve(BattleElementData->Phases.Num());

	for (const FBattleElementEventPhase& PhaseTemplate : BattleElementData->Phases)
	{
		FBattleElementEventPhase& RuntimePhase = Phases.AddDefaulted_GetRef();

		RuntimePhase.Events.Reserve(PhaseTemplate.Events.Num());
		for (UBattleEvent* EventTemplate : PhaseTemplate.Events)
		{
			if (!EventTemplate) continue;

			UBattleEvent* RuntimeEvent = DuplicateObject<UBattleEvent>(EventTemplate, this);

			RuntimePhase.Events.Add(RuntimeEvent);
		}
	}
}

void UBattleElement::EvaluatePhases(float DeltaTime)
{
	if (CurrentPhaseIndex < Phases.Num())
	{
		if (CurrentPhase.HasEvents() && !CurrentPhase.IsRunning())
		{
			CurrentPhase = Phases[CurrentPhaseIndex];
			StartCurrentPhase();
		}
	}
}

void UBattleElement::StartCurrentPhase()
{
	CurrentPhase.CurrentEventIndex = 0;
	// TODO: start event, if complete, advance current phase
}

void UBattleElement::AdvanceCurrentPhaseEvent()
{
	CurrentPhase.CurrentEventIndex++;

	if (CurrentPhase.IsComplete())
	{
		CompleteCurrentPhase();
	}
}

void UBattleElement::CompleteCurrentPhase()
{
	CurrentPhaseIndex++;

	if (CurrentPhaseIndex >= Phases.Num())
	{
		End();
	}
}
