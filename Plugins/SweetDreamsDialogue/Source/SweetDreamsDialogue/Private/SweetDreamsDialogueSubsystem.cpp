

#include "SweetDreamsDialogueSubsystem.h"
#include "DialogueData.h"

USweetDreamsDialogueSubsystem::USweetDreamsDialogueSubsystem() {}

void USweetDreamsDialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USweetDreamsDialogueSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USweetDreamsDialogueSubsystem::StartDialogue(UDialogueData* Dialogue)
{
	ResetDialogueData();

	if (IsValid(Dialogue))
	{
		DialogueData = Dialogue;
		Dialogues = DialogueData->Dialogues;

		if (!Dialogues.IsEmpty())
		{
			CurrentDialogue = Dialogues[0];
			bIsDialogueActive = true;
			// delegate

			ProcessDialogue(CurrentDialogue);
		}
		else
		{
			EndDialogue();
		}
	}

}

void USweetDreamsDialogueSubsystem::UpdateDialogue()
{
	if (bUseAnimatedDialogue && bIsAnimating)
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

	CurrentDialogueID = FMath::Clamp(++CurrentDialogueID, 0, Dialogues.Num());
	CurrentDialogue = Dialogues[CurrentDialogueID];
	EDialogueMode CurrentMode = CurrentDialogue.Mode;
	switch (CurrentMode)
	{
	case EDialogueMode::DIALOGUE:
		ProcessDialogue(CurrentDialogue);
		break;
	case EDialogueMode::SEQUENCE:
		//start sequence;
		break;
	default:
		break;
	}
}

void USweetDreamsDialogueSubsystem::ProcessDialogue(FSweetDreamsDialogue Dialogue)
{
	FullDialogueBody = Dialogue.DialogueBody.ToString();
	if (bUseAnimatedDialogue)
	{
		AnimatedDialogueBody = FText::GetEmpty();
		ProcessedDialogueBody.Empty();
		for (int32 i = 0; i < FullDialogueBody.Len();)
		{
			if (FullDialogueBody[i] == '<')
			{
				ProcessRichTextTags(i);
			}
			else
			{
				ProcessedDialogueBody.AppendChar(FullDialogueBody[i++]);
			}
		}
		CurrentLetterIndex = 0;
		LetterDisplayElapsed = 0.f;
		bIsAnimating = true;
	}

	AddDialogueToLog(Dialogue);

	if (Dialogue.DialogueAudio)
	{
		// play audio delegate
	}

	bIsSelectingChoices = Dialogue.Choices.Num() > 0;
	if (bIsSelectingChoices)
	{
		// show choices delegate
	}
	else
	{
		// not showing choices delegate
	}
}

void USweetDreamsDialogueSubsystem::AddDialogueToLog(FSweetDreamsDialogue Dialogue)
{
	FText LogText = Dialogue.DialogueBody;
	FText LogName = Dialogue.DialogueName;
	FSweetDreamsDialogueLog Log = FSweetDreamsDialogueLog(LogText, LogName, FText());
	DialogueLog.Add(Log);
}


void USweetDreamsDialogueSubsystem::ProcessRichTextTags(int32& LetterIndex)
{

}

void USweetDreamsDialogueSubsystem::SkipAnimatedDialogue()
{
	bIsAnimating = false;
	CurrentLetterIndex = ProcessedDialogueBody.Len();
}

void USweetDreamsDialogueSubsystem::SelectChoiceAndUpdate(FChoice Choice)
{
	DialogueLog[CurrentDialogueID].UpdateChoice(Choice.ChoiceBody);
	if (DialogueData->ChoiceResults.Contains(Choice.ChoiceResult))
	{
		Dialogues.Append(DialogueData->ChoiceResults[Choice.ChoiceResult].Dialogues);
	}
	bIsSelectingChoices = false;
	UpdateDialogue();
}

void USweetDreamsDialogueSubsystem::InsertDialogue(UDialogueData* Dialogue, int32 Index)
{
	if (IsValid(Dialogue))
	{
		TArray<FSweetDreamsDialogue> NewDialogue = Dialogue->Dialogues;
		if (Index > 0)
		{
			Dialogues.Insert(NewDialogue, Index);
		}
		else
		{
			Dialogues.Append(NewDialogue);
		}
	}
}

void USweetDreamsDialogueSubsystem::ResetDialogueData()
{
	DialogueData = nullptr;
	Dialogues.Empty();
	DialogueLog.Empty();
	CurrentDialogue = FSweetDreamsDialogue();
	CurrentDialogueID = 0;
	bIsAnimating = false;
	bIsSelectingChoices = false;
}

void USweetDreamsDialogueSubsystem::EndDialogue()
{
	ResetDialogueData();
	bIsDialogueActive = false;
	// delegate
}

