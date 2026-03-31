

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

FSweetDreamsDialogueLog USweetDreamsDialogueSubsystem::ConvertDialogueToLog(const FSweetDreamsDialogue& Dialogue, const FChoice SelectedChoice) const
{
	FText LogText = Dialogue.Body;
	FText LogName = Dialogue.SpeakerName;

	return FSweetDreamsDialogueLog(LogText, LogName, SelectedChoice.Body);
}

void USweetDreamsDialogueSubsystem::InsertDialogue(TArray<FSweetDreamsDialogue>& CurrentDialogue, const UDialogueData* ToInsert, int32 Index)
{
	if (IsValid(ToInsert))
	{
		TArray<FSweetDreamsDialogue> NewDialogue = ToInsert->Dialogues;

		if (Index > 0)
		{
			CurrentDialogue.Insert(NewDialogue, Index);
		}
		else
		{
			CurrentDialogue.Append(NewDialogue);
		}
	}
}

void USweetDreamsDialogueSubsystem::SelectChoice(const FChoice& Choice)
{

}

FString USweetDreamsDialogueSubsystem::GetTaglessAnimatedDialogue(const FSweetDreamsDialogue& Dialogue) const
{
	FString FullBody = Dialogue.Body.ToString();
	FString TaglessDialogueBody;

	for (int32 i = 0; i < FullBody.Len();)
	{
		if (FullBody[i] == '<')
		{
			int32 TagEnd = FullBody.Find(TEXT(">"), ESearchCase::IgnoreCase, ESearchDir::FromStart, i);

			if (TagEnd != INDEX_NONE)
			{
				i = TagEnd + 1;
			}
		}
		else
		{
			TaglessDialogueBody.AppendChar(FullBody[i++]);
		}
	}

	return TaglessDialogueBody;
}

FText USweetDreamsDialogueSubsystem::GetAnimatedDialogue(const FSweetDreamsDialogue& Dialogue, const int32& LetterIndex) const
{
	FString FullBody = Dialogue.Body.ToString();
	FString DisplayText;
	DisplayText.Reserve(FullBody.Len());

	int32 VisibleChars = 0;

	for (int32 i = 0; i < FullBody.Len(); ++i)
	{
		if (FullBody[i] == '<')
		{
			const int32 TagEnd = FullBody.Find(TEXT(">"), ESearchCase::IgnoreCase, ESearchDir::FromStart, i);

			if (TagEnd != INDEX_NONE)
			{
				DisplayText.Append(FullBody.Mid(i, TagEnd - i + 1));
				i = TagEnd;
				continue;
			}
		}

		if (VisibleChars < LetterIndex)
		{
			DisplayText.AppendChar(FullBody[i]);
			++VisibleChars;
		}
	}

	return FText::FromString(DisplayText);
}
