

#include "SweetDreamsAIStateBehaviour.h"

bool USweetDreamsAIStateBehaviour::CanEnterState_Implementation() const
{
    return true;
}

void USweetDreamsAIStateBehaviour::SetOwner(AActor* InOwner)
{
    if (!IsValid(InOwner)) return;
    StateOwner = InOwner;
}

AActor* USweetDreamsAIStateBehaviour::GetOwner() const
{
    return StateOwner;
}

void USweetDreamsAIStateBehaviour::SetState(USweetDreamsAIState* Data)
{
    StateData = Data;
}

USweetDreamsAIState* USweetDreamsAIStateBehaviour::GetStateData() const
{
    return StateData;
}


