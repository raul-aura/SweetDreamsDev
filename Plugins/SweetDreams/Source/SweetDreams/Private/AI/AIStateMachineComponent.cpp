


#include "AI/AIStateMachineComponent.h"
#include "AI/SweetDreamsAIState.h"
#include "AI/SweetDreamsAIStateBehaviour.h"

UAIStateMachineComponent::UAIStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAIStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAIStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(CurrentBehaviour))
	{
		CurrentBehaviour->Tick(DeltaTime);
	}
}

bool UAIStateMachineComponent::AddState(USweetDreamsAIState* InState, USweetDreamsAIStateBehaviour*& Behaviour, int32& Index, bool bEnterStateOnAdd)
{
	if (IsValid(InState) && IsValid(InState->BehaviourClass))
	{
		bool bAdded = false;
		if (States.Contains(InState))
		{
			Index = States.IndexOfByKey(InState);
		}
		else
		{
			Index = States.Add(InState);
			auto* NewBehaviour = NewObject<USweetDreamsAIStateBehaviour>(GetOwner(), InState->BehaviourClass, InState->StateName);
			if (IsValid(NewBehaviour))
			{
				NewBehaviour->SetOwner(GetOwner());
				Behaviours.Add(InState->StateName, NewBehaviour);
				Behaviour = NewBehaviour;
			}
			bAdded = true;
		}
		if (bEnterStateOnAdd)
		{
			SetState(InState, Behaviour);
		}
		return bAdded;
	}
	return false;
}

bool UAIStateMachineComponent::SetState(USweetDreamsAIState* InState, USweetDreamsAIStateBehaviour*& Behaviour)
{
	ClearCurrentState();
	if (IsValid(InState) && IsValid(InState->BehaviourClass))
	{
		CurrentState = InState;
		if (USweetDreamsAIStateBehaviour** Found = Behaviours.Find(InState->StateName))
		{
			CurrentBehaviour = *Found;
			Behaviour = CurrentBehaviour;
			return CurrentBehaviour->OnEnter();
		}
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


