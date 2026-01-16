

#include "SweetDreamsDialogueSubsystem.h"
#include "DialogueData.h"
#include "DialogueComponent.h"

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
			OnDialogueStarted.Broadcast();

			ProcessDialogue(CurrentDialogue, 0);
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
	OnDialogueUpdated.Broadcast(CurrentDialogue, CurrentDialogueID);
	CallCustomFunctions(CurrentDialogue, CurrentDialogueID);

	EDialogueMode CurrentMode = CurrentDialogue.Mode;
	switch (CurrentMode)
	{
	case EDialogueMode::DIALOGUE:
		ProcessDialogue(CurrentDialogue, CurrentDialogueID);
		break;
	case EDialogueMode::SEQUENCE:
		OnDialogueSequence.Broadcast(CurrentDialogue, CurrentDialogueID, CurrentDialogue.DialogueSequence);
		break;
	case EDialogueMode::CUSTOM:
		OnCustomDialogueMode.Broadcast(CurrentDialogue, CurrentDialogueID);
		break;
	default:
		break;
	}
}

void USweetDreamsDialogueSubsystem::UpdateAnimatedDialogue(float DeltaTime)
{
	if (bUseAnimatedDialogue && bIsAnimating)
	{
		LetterDisplayElapsed += DeltaTime;

		if (CanAdvanceLetters())
		{
			BuildAnimatedDialogue();

			if (CurrentLetterIndex >= TaglessDialogueBody.Len())
			{
				bIsAnimating = false;

				OnDialogueAnimationFinished.Broadcast(CurrentDialogue, CurrentDialogueID, AnimatedDialogueBody);
			}
		}
	}
}

void USweetDreamsDialogueSubsystem::ProcessDialogue(FSweetDreamsDialogue Dialogue, int32 Index)
{
	if (bUseAnimatedDialogue)
	{
		ProcessAnimatedDialogue(Dialogue);
	}

	AddDialogueToLog(Dialogue);

	if (Dialogue.DialogueAudio)
	{
		OnDialogueAudio.Broadcast(Dialogue.DialogueAudio);
	}

	bIsSelectingChoices = Dialogue.Choices.Num() > 0;
	if (bIsSelectingChoices)
	{
		OnDialogueChoices.Broadcast(Dialogue, Index, Dialogue.Choices);
	}
	else
	{
		OnDialogueNoChoices.Broadcast();
	}
}

void USweetDreamsDialogueSubsystem::AddDialogueToLog(FSweetDreamsDialogue Dialogue)
{
	FText LogText = Dialogue.DialogueBody;
	FText LogName = Dialogue.DialogueName;
	FSweetDreamsDialogueLog Log = FSweetDreamsDialogueLog(LogText, LogName, FText());
	DialogueLog.Add(Log);
}

void USweetDreamsDialogueSubsystem::CallCustomFunctions(FSweetDreamsDialogue Dialogue, int32 Index)
{
	if (!Dialogue.FunctionsToCall.IsEmpty())
	{
		OnDialogueCustomFunction.Broadcast(Dialogue, Index, Dialogue.FunctionsToCall);
	}
}

void USweetDreamsDialogueSubsystem::SkipAnimatedDialogue()
{
	CurrentLetterIndex = TaglessDialogueBody.Len();
	bIsAnimating = false;
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
	OnDialogueEnded.Broadcast();
}

void USweetDreamsDialogueSubsystem::SkipRichTextTags(int32& LetterIndex)
{
	int32 TagEnd = FullDialogueBody.Find(TEXT(">"), ESearchCase::IgnoreCase, ESearchDir::FromStart, LetterIndex);
	if (TagEnd != INDEX_NONE)
	{
		LetterIndex = TagEnd + 1;
	}
}

void USweetDreamsDialogueSubsystem::ProcessAnimatedDialogue(FSweetDreamsDialogue Dialogue)
{
	FullDialogueBody = Dialogue.DialogueBody.ToString();
	CurrentAnimatedSettings = CurrentDialogue.AnimatedSettings;
	AnimatedDialogueBody = FText::GetEmpty();
	TaglessDialogueBody.Empty();

	for (int32 i = 0; i < FullDialogueBody.Len();)
	{
		if (FullDialogueBody[i] == '<')
		{
			SkipRichTextTags(i);
		}
		else
		{
			TaglessDialogueBody.AppendChar(FullDialogueBody[i++]);
		}
	}
	CurrentLetterIndex = 0;
	LetterDisplayElapsed = 0.f;
	bIsAnimating = true;
}

bool USweetDreamsDialogueSubsystem::CanAdvanceLetters()
{
	const int32 LettersToAdvance = FMath::FloorToInt(LetterDisplayElapsed / CurrentAnimatedSettings.LetterDisplayRate);

	if (LettersToAdvance <= 0)
	{
		return false;
	}

	CurrentLetterIndex = FMath::Min(CurrentLetterIndex + LettersToAdvance,TaglessDialogueBody.Len());

	LetterDisplayElapsed = 0.f;
	return true;
}

void USweetDreamsDialogueSubsystem::BuildAnimatedDialogue()
{
	FString DisplayText;
	DisplayText.Reserve(FullDialogueBody.Len());

	int32 VisibleChars = 0;
	const int32 MaxVisible = CurrentLetterIndex;

	for (int32 i = 0; i < FullDialogueBody.Len(); ++i)
	{
		if (FullDialogueBody[i] == '<')
		{
			const int32 TagEnd =
				FullDialogueBody.Find(TEXT(">"), ESearchCase::IgnoreCase, ESearchDir::FromStart, i);

			if (TagEnd != INDEX_NONE)
			{
				DisplayText.Append(FullDialogueBody.Mid(i, TagEnd - i + 1));
				i = TagEnd;
				continue;
			}
		}

		if (VisibleChars < MaxVisible)
		{
			DisplayText.AppendChar(FullDialogueBody[i]);
			++VisibleChars;
		}
	}

	AnimatedDialogueBody = FText::FromString(DisplayText);
	OnDialogueAnimating.Broadcast(CurrentDialogue, CurrentDialogueID, AnimatedDialogueBody);
}

UDialogueData* USweetDreamsDialogueSubsystem::GetDialogueData() const
{
	return DialogueData;
}

void USweetDreamsDialogueSubsystem::SetDialogueData(UDialogueData* InData)
{
	ResetDialogueData();
	DialogueData = InData;
}

TArray<FSweetDreamsDialogue> USweetDreamsDialogueSubsystem::GetDialogues() const
{
	return Dialogues;
}

TArray<FSweetDreamsDialogueLog> USweetDreamsDialogueSubsystem::GetDialogueLog() const
{
	return DialogueLog;
}

FSweetDreamsDialogue USweetDreamsDialogueSubsystem::GetCurrentDialogue(int32& Index) const
{
	Index = CurrentDialogueID;
	return CurrentDialogue;
}

int32 USweetDreamsDialogueSubsystem::GetCurrentDialogueID() const
{
	return CurrentDialogueID;
}

bool USweetDreamsDialogueSubsystem::IsUsingAnimatedDialogue() const
{
	return bUseAnimatedDialogue;
}

void USweetDreamsDialogueSubsystem::SetUseAnimatedDialogue(bool bInUseAnimatedDialogue)
{
	bUseAnimatedDialogue = bInUseAnimatedDialogue;
}
