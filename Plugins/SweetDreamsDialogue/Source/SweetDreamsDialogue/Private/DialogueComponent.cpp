

#include "DialogueComponent.h"
#include "SweetDreamsDialogueSubsystem.h"
#include "DialogueData.h"

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!bDialogueInExecution || !bIsAnimating) return;

	LetterDisplayElapsed += DeltaTime;

	const int32 LettersToAdvance = FMath::FloorToInt(LetterDisplayElapsed / CurrentAnimatedSettings.LetterDisplayRate);

	if (LettersToAdvance <= 0) return;

	CurrentLetterIndex = FMath::Min(CurrentLetterIndex + LettersToAdvance,TaglessDialogueBody.Len());

	LetterDisplayElapsed = 0.f;

	BuildAnimatedDialogue();

	if (CurrentLetterIndex >= TaglessDialogueBody.Len())
	{
		bIsAnimating = false;
		//OnDialogueAnimationFinished.Broadcast();
	}
}

void UDialogueComponent::SetDialogueData(UDialogueData* InData)
{
	DialogueData = InData;
}

void UDialogueComponent::StartDialogue(UDialogueData* Dialogue)
{
	if (bDialogueInExecution || !IsValid(Dialogue)) return;

	SetDialogueData(Dialogue);
	Dialogues = DialogueData->Dialogues;

	if (Dialogues.IsEmpty()) return;

	bDialogueInExecution = true;
	CurrentDialogueID = 0;

	OnDialogueStarted.Broadcast();

	ProcessDialogue();
}

void UDialogueComponent::UpdateDialogue()
{
	if (bIsAnimating)
	{
		SkipAnimatedDialogue();
		return;
	}

	if (bIsSelectingChoices) return;

	if (CurrentDialogueID >= Dialogues.Num() - 1)
	{
		EndDialogue();
		return;
	}

	CurrentDialogueID++;
	ProcessDialogue();
}

void UDialogueComponent::SkipAnimatedDialogue()
{
	CurrentLetterIndex = TaglessDialogueBody.Len();
	bIsAnimating = false;

	BuildAnimatedDialogue();
	//OnDialogueAnimationFinished.Broadcast();
}

void UDialogueComponent::SelectChoiceAndUpdate(FChoice Choice)
{

}

USweetDreamsDialogueSubsystem* UDialogueComponent::GetDialogueSubsystem() const
{
	return GetWorld()->GetGameInstance()->GetSubsystem<USweetDreamsDialogueSubsystem>();
}

void UDialogueComponent::ProcessDialogue()
{
	CurrentDialogue = Dialogues[CurrentDialogueID];

	DialogueLog.Add(FSweetDreamsDialogueLog(CurrentDialogue.Body, CurrentDialogue.SpeakerName, FText()));

	OnDialogueUpdated.Broadcast(CurrentDialogue, CurrentDialogueID);

	StartAnimatedDialogue(CurrentDialogue);

	bIsSelectingChoices = CurrentDialogue.Choices.Num() > 0;
}

void UDialogueComponent::EndDialogue()
{
	bDialogueInExecution = false;

	DialogueData = nullptr;
	Dialogues.Empty();
	DialogueLog.Empty();

	OnDialogueEnded.Broadcast();
}

void UDialogueComponent::StartAnimatedDialogue(const FSweetDreamsDialogue& Dialogue)
{
	if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
	{
		TaglessDialogueBody = Subsystem->GetTaglessAnimatedDialogue(Dialogue);
	}

	CurrentLetterIndex = 0;
	LetterDisplayElapsed = 0.f;
	bIsAnimating = true;
}

void UDialogueComponent::BuildAnimatedDialogue()
{
	if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
	{
		AnimatedDialogueBody = Subsystem->GetAnimatedDialogue(CurrentDialogue, CurrentLetterIndex);
	}

	//OnDialogueAnimating.Broadcast(AnimatedDialogueBody);
}


