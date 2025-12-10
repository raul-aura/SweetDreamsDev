

#include "SweetDreamsAIStateBehaviour.h"

bool USweetDreamsAIStateBehaviour::OnEnter_Implementation()
{
    bIsStateActive = CanEnterState();
    return bIsStateActive;
}

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


