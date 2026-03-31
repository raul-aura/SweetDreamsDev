

#include "AIStateMachineComponent.h"
#include "SweetDreamsAIState.h"
#include "SweetDreamsAIStateBehaviour.h"

UAIStateMachineComponent::UAIStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAIStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState.Behaviour)
	{
		CurrentState.Behaviour->Tick(DeltaTime);
	}
}

void UAIStateMachineComponent::CreateInitialStates()
{
	for (TObjectPtr<USweetDreamsAIState> State : InitialStates)
	{
		int32 Index = INDEX_NONE;
		FSweetDreamsStateRuntime TempState;

		AddState(State, TempState, Index, false);
	}

	OnInitialStatesCreated();
}

bool UAIStateMachineComponent::AddState(USweetDreamsAIState* StateData, FSweetDreamsStateRuntime& OutState, int32& Index, bool bEnterStateOnAdd)
{
	if (IsValid(StateData))
	{
		bool bAdded = false;

		for (int32 i = 0; i < States.Num(); i++)
		{
			if (States.IsValidIndex(i))
			{
				const FSweetDreamsStateRuntime& State = States[i];

				if (State.Data && State.Data->StateTag.MatchesTagExact(StateData->StateTag))
				{
					Index = i;
					OutState = State;
					return false;
				}
			}
		}

		USweetDreamsAIStateBehaviour* NewBehaviour = nullptr;

		if (IsValid(StateData->Behaviour))
		{
			NewBehaviour = DuplicateObject<USweetDreamsAIStateBehaviour>(StateData->Behaviour, this);
			if (IsValid(NewBehaviour))
			{
				NewBehaviour->SetOwner(GetOwner());
				NewBehaviour->SetState(StateData);
				NewBehaviour->OnInitialized();
			}
		}

		const FSweetDreamsStateRuntime State(StateData, NewBehaviour);

		Index = States.Add(State);
		bAdded = true;

		if (bEnterStateOnAdd)
		{
			FSweetDreamsStateRuntime TempState;
			SetState(StateData->StateTag, TempState);
		}

		return bAdded;
	}

	return false;
}

bool UAIStateMachineComponent::SetState(FGameplayTag InStateTag, FSweetDreamsStateRuntime& OutState)
{
	if (!CanUpdateCurrentState()) return false;

	ClearCurrentState();

	for (const FSweetDreamsStateRuntime& State : States)
	{
		if (State.Data && State.Data->StateTag.MatchesTagExact(InStateTag))
		{
			CurrentState = State;
			OutState = State;

			if (IsValid(State.Behaviour) && State.Behaviour->CanEnterState())
			{
				State.Behaviour->OnEnter();
			}

			return true;
		}
	}

	return false;
}

void UAIStateMachineComponent::ClearCurrentState()
{
	if (IsValid(CurrentState.Behaviour))
	{
		CurrentState.Behaviour->OnExit();
	}

	CurrentState = FSweetDreamsStateRuntime();
}

bool UAIStateMachineComponent::GetStateByTag(FGameplayTag InStateTag, FSweetDreamsStateRuntime& OutState) const
{
	for (const FSweetDreamsStateRuntime& State : States)
	{
		if (State.Data && State.Data->StateTag.MatchesTagExact(InStateTag))
		{
			OutState = State;
			return true;
		}
	}

	return false;
}

float UAIStateMachineComponent::GetStateDuration(FGameplayTag InStateTag) const
{
	for (const FSweetDreamsStateRuntime& State : States)
	{
		if (State.Data && State.Data->StateTag.MatchesTagExact(InStateTag))
		{
			if (State.Data->bUseRandomizedDuration)
			{
				return FMath::RandRange(State.Data->MinDuration, State.Data->MaxDuration);
			}

			return State.Data->Duration;
		}
	}

	return INDEX_NONE;
}

USweetDreamsAIStateBehaviour* UAIStateMachineComponent::GetStateBehaviour(FGameplayTag InStateTag) const
{
	for (const FSweetDreamsStateRuntime& State : States)
	{
		if (State.Data && State.Data->StateTag.MatchesTagExact(InStateTag))
		{
			return State.Behaviour;
		}
	}

	return nullptr;
}

bool UAIStateMachineComponent::IsInState(FGameplayTag InStateTag) const
{
	if (!CurrentState.Data) return false;

	return CurrentState.Data->StateTag.MatchesTagExact(InStateTag);
}

bool UAIStateMachineComponent::CanUpdateCurrentState_Implementation() const
{
	return true;
}


