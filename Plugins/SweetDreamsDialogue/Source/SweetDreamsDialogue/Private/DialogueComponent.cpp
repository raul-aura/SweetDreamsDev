

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
		SkipAnimatedDialogue();
	}
}

void UDialogueComponent::StartDialogue(UDialogueData* Dialogue)
{
	if (bDialogueInExecution || !IsValid(Dialogue)) return;

	DialogueData = Dialogue;
	Dialogues = DialogueData->Dialogues;

	if (Dialogues.IsEmpty()) return;

	bDialogueInExecution = true;
	CurrentDialogueID = -1;

	OnDialogueStarted.Broadcast();
}

void UDialogueComponent::ProcessDialogue()
{
	CurrentDialogueID++;
	CurrentDialogue = Dialogues[CurrentDialogueID];

	StartAnimatedDialogue(CurrentDialogue);

	bIsSelectingChoices = !CurrentDialogue.Choices.IsEmpty();

	if (!bIsSelectingChoices)
	{
		if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
		{
			FSweetDreamsDialogueLog Log = Subsystem->ConvertDialogueToLog(CurrentDialogue, FText());
			DialogueLog.Add(Log);
		}
	}
}

bool UDialogueComponent::CanAdvanceDialogue()
{
	if (bIsAnimating || bIsSelectingChoices || ShouldDialogueEnd())
	{
		return false;
	}

	return true;
}

bool UDialogueComponent::ShouldDialogueEnd() const
{
	return CurrentDialogueID >= Dialogues.Num() - 1;
}

void UDialogueComponent::SkipAnimatedDialogue()
{
	const bool bWasAnimating = bIsAnimating;

	CurrentLetterIndex = TaglessDialogueBody.Len();
	bIsAnimating = false;

	if (bWasAnimating)
	{
		BuildAnimatedDialogue();
	}
}

TArray<FSweetDreamsDialogue>& UDialogueComponent::GetDialogueArray()
{
	return Dialogues;
}

const FSweetDreamsDialogue& UDialogueComponent::GetCurrentDialogue() const
{
	return CurrentDialogue;
}

int32 UDialogueComponent::GetCurrentDialogueIndex() const
{
	return CurrentDialogueID;
}

bool UDialogueComponent::IsInExecution() const
{
	return bDialogueInExecution;
}

FText UDialogueComponent::GetCurrentAnimatedText() const
{
	return AnimatedDialogueBody;
}

bool UDialogueComponent::IsAnimatingDialogue() const
{
	return bIsAnimating;
}

void UDialogueComponent::SelectChoice(FGameplayTag Choice)
{
	if (const FText* SelectedChoice = CurrentDialogue.Choices.Find(Choice))
	{
		if (const FChoiceDialogue* ChoiceDialogues = DialogueData->ChoiceResults.Find(Choice))
		{
			Dialogues.Insert(ChoiceDialogues->Dialogues, ChoiceDialogues->IndexToInsert);

			if (USweetDreamsDialogueSubsystem* Subsystem = GetDialogueSubsystem())
			{
				FSweetDreamsDialogueLog Log = Subsystem->ConvertDialogueToLog(CurrentDialogue, *SelectedChoice);
				DialogueLog.Add(Log);
			}

			bIsSelectingChoices = false;
		}
	}
}

void UDialogueComponent::EndDialogue()
{
	if (bDialogueInExecution)
	{
		bDialogueInExecution = false;

		DialogueData = nullptr;
		Dialogues.Empty();
		DialogueLog.Empty();
		CurrentDialogue = FSweetDreamsDialogue();
		CurrentDialogueID = -1;

		bIsAnimating = false;
		TaglessDialogueBody = "";
		AnimatedDialogueBody = FText::FromString("");

		OnDialogueEnded.Broadcast();
	}
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
}

USweetDreamsDialogueSubsystem* UDialogueComponent::GetDialogueSubsystem() const
{
	return GetWorld()->GetSubsystem<USweetDreamsDialogueSubsystem>();
}


