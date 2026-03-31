


#include "AIStateMachineComponent.h"
#include "SweetDreamsAIState.h"
#include "SweetDreamsAIStateBehaviour.h"

UAIStateMachineComponent::UAIStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAIStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TObjectPtr<USweetDreamsAIState> State : InitialStates)
	{
		int32 Index = INDEX_NONE;
		USweetDreamsAIStateBehaviour* Behaviour = nullptr;

		AddState(State, Behaviour, Index, false);
	}
}

void UAIStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(CurrentBehaviour))
	{
		CurrentBehaviour->Tick(DeltaTime);
	}
}

bool UAIStateMachineComponent::AddState(USweetDreamsAIState* InState, USweetDreamsAIStateBehaviour*& OutBehaviour, int32& Index, bool bEnterStateOnAdd)
{
	if (IsValid(InState))
	{
		bool bAdded = false;

		if (States.Contains(InState))
		{
			Index = States.IndexOfByKey(InState);
			return false;
		}
		else
		{
			Index = States.Add(InState);
			bAdded = true;

			if (IsValid(InState->Behaviour))
			{
				TObjectPtr<USweetDreamsAIStateBehaviour> NewBehaviour = DuplicateObject<USweetDreamsAIStateBehaviour>(InState->Behaviour, this, InState->StateName);
				if (IsValid(NewBehaviour))
				{
					NewBehaviour->SetOwner(GetOwner());
					Behaviours.Add(InState->StateName, NewBehaviour);
					OutBehaviour = NewBehaviour;
				}
			}
		}

		if (bEnterStateOnAdd)
		{
			SetState(InState, OutBehaviour);
		}

		return bAdded;
	}

	return false;
}

bool UAIStateMachineComponent::SetState(USweetDreamsAIState* InState, USweetDreamsAIStateBehaviour*& OutBehaviour)
{
	ClearCurrentState();

	if (IsValid(InState))
	{
		CurrentState = InState;

		if (IsValid(InState->Behaviour))
		{
			if (TObjectPtr<USweetDreamsAIStateBehaviour>* Found = Behaviours.Find(InState->StateName))
			{
				USweetDreamsAIStateBehaviour* BehaviourPtr = Found->Get();
				if (BehaviourPtr && BehaviourPtr->CanEnterState())
				{
					CurrentBehaviour = *Found;
					CurrentBehaviour->OnEnter();
					OutBehaviour = CurrentBehaviour.Get();

					return true;
				}
			}
		}

		return true;
	}

	return false;
}

void UAIStateMachineComponent::ClearCurrentState()
{
	if (IsValid(CurrentBehaviour))
	{
		CurrentBehaviour->OnExit();
		CurrentBehaviour = nullptr;
	}

	CurrentState = nullptr;
}


