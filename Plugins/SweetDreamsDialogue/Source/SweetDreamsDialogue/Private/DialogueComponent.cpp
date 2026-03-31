

#include "DialogueComponent.h"
#include "SweetDreamsDialogueSubsystem.h"
#include "DialogueData.h"

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!bDialogueInExecution) return;

	if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
	{
		Subsystem->UpdateAnimatedDialogue(DeltaTime);
	}
}

void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogueComponent::SetDialogueData(UDialogueData* InData)
{
	if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
	{
		Subsystem->SetDialogueData(InData);
	}
}

void UDialogueComponent::StartDialogue(UDialogueData* Dialogue)
{
	if (bDialogueInExecution) return;
	
	if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
	{
		BindDelegates();

		bDialogueInExecution = true;

		Subsystem->StartDialogue(Dialogue);
	}
}

void UDialogueComponent::UpdateDialogue()
{
	if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
	{
		Subsystem->UpdateDialogue();
	}
}

void UDialogueComponent::SkipAnimatedDialogue()
{
	if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
	{
		Subsystem->SkipAnimatedDialogue();
	}
}

void UDialogueComponent::SelectChoiceAndUpdate(FChoice Choice)
{
	if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
	{
		Subsystem->SelectChoiceAndUpdate(Choice);
	}
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
		Subsystem->OnDialogueEnded.AddUniqueDynamic(this, &UDialogueComponent::BroadcastDialogueEnded);
		Subsystem->OnDialogueUpdated.AddUniqueDynamic(this, &UDialogueComponent::BroadcastDialogueUpdated);
	}
}

void UDialogueComponent::BroadcastDialogueStarted()
{
	OnDialogueStarted.Broadcast();
}

void UDialogueComponent::BroadcastDialogueEnded()
{
	bDialogueInExecution = false;

	OnDialogueEnded.Broadcast();
}

void UDialogueComponent::BroadcastDialogueUpdated(FSweetDreamsDialogue Dialogue, int32 Index)
{
	OnDialogueUpdated.Broadcast(Dialogue, Index);
}

