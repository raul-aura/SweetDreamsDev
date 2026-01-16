

#include "DialogueComponent.h"
#include "SweetDreamsDialogueSubsystem.h"
#include "DialogueData.h"

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
	{
		Subsystem->UpdateAnimatedDialogue(DeltaTime);
	}
}

void UDialogueComponent::BeginPlay()
{
	BindDelegates();
	Super::BeginPlay();
}

void UDialogueComponent::StartDialogue(UDialogueData* Dialogue)
{

}

void UDialogueComponent::UpdateDialogue()
{

}

void UDialogueComponent::SkipAnimatedDialogue()
{

}

void UDialogueComponent::SelectChoiceAndUpdate(FChoice Choice)
{

}

USweetDreamsDialogueSubsystem* UDialogueComponent::GetDialogueSubsystem() const
{
	return GetWorld()->GetGameInstance()->GetSubsystem<USweetDreamsDialogueSubsystem>();
}

void UDialogueComponent::BindDelegates()
{
	if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
	{
		Subsystem->OnDialogueStarted.AddUniqueDynamic(this, &UDialogueComponent::BroadcastDialogueStarted);
		Subsystem->OnDialogueUpdated.AddUniqueDynamic(this, &UDialogueComponent::BroadcastDialogueUpdated);
	}
}

void UDialogueComponent::BroadcastDialogueStarted()
{
	OnDialogueStarted.Broadcast();
}

void UDialogueComponent::BroadcastDialogueEnded()
{
	OnDialogueEnded.Broadcast();
}

void UDialogueComponent::BroadcastDialogueUpdated(FSweetDreamsDialogue Dialogue, int32 Index)
{
	OnDialogueUpdated.Broadcast(Dialogue, Index);
}

