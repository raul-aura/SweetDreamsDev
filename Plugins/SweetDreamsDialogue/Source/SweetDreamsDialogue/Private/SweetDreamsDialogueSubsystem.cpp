

#include "SweetDreamsDialogueSubsystem.h"
#include "DialogueData.h"
#include "DialogueComponent.h"

USweetDreamsDialogueSubsystem::USweetDreamsDialogueSubsystem() {}

void USweetDreamsDialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

USweetDreamsDialogueSubsystem* USweetDreamsDialogueSubsystem::Get(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);

	return World->GetSubsystem<USweetDreamsDialogueSubsystem>();
}

void USweetDreamsDialogueSubsystem::StartDialogue(UDialogueComponent* Component, UDialogueData* Dialogue)
{
	if (IsValid(Component))
	{
		CurrentDialogueComponent = Component;

		Component->StartDialogue(Dialogue);

		OnDialogueStarted.Broadcast();

		UpdateDialogue();
	}
}

void USweetDreamsDialogueSubsystem::UpdateDialogue()
{
	if (UDialogueComponent* Component = CurrentDialogueComponent.Get())
	{
		if (Component->CanAdvanceDialogue())
		{
			Component->ProcessDialogue();

			const FSweetDreamsDialogue& CurrentDialogue = Component->GetCurrentDialogue();
			OnDialogueUpdated.Broadcast(CurrentDialogue);

			if (!CurrentDialogue.Choices.IsEmpty())
			{
				TArray<FChoice> Choices;
				Choices.Reserve(CurrentDialogue.Choices.Num());

				for (const TPair<FGameplayTag, FText>& Pair : CurrentDialogue.Choices)
				{
					Choices.Emplace(Pair.Key, Pair.Value);
				}

				OnDialogueChoices.Broadcast(Choices);
			}
		}
		else if (Component->IsAnimatingDialogue())
		{
			Component->SkipAnimatedDialogue();
		}
		else if (Component->ShouldDialogueEnd())
		{
			EndDialogue();
		}
	}
}

void USweetDreamsDialogueSubsystem::EndDialogue()
{
	if (UDialogueComponent* Component = CurrentDialogueComponent.Get())
	{
		Component->EndDialogue();

		OnDialogueEnded.Broadcast();
	}

	CurrentDialogueComponent = nullptr;
}

void USweetDreamsDialogueSubsystem::SelectChoiceAndUpdate(FGameplayTag Choice)
{
	if (UDialogueComponent* Component = CurrentDialogueComponent.Get())
	{
		Component->SelectChoice(Choice);

		UpdateDialogue();
	}
}

void USweetDreamsDialogueSubsystem::SkipAnimatedDialogue()
{
	if (UDialogueComponent* Component = CurrentDialogueComponent.Get())
	{
		Component->SkipAnimatedDialogue();

		OnDialogueAnimationFinished.Broadcast(Component->GetCurrentAnimatedText());
	}
}

bool USweetDreamsDialogueSubsystem::IsAnyDialogueInExecution() const
{
	return CurrentDialogueComponent.IsValid();
}

FSweetDreamsDialogueLog USweetDreamsDialogueSubsystem::ConvertDialogueToLog(const FSweetDreamsDialogue& Dialogue, const FText SelectedChoice) const
{
	FText LogText = Dialogue.Body;
	FText LogName = Dialogue.SpeakerName;

	return FSweetDreamsDialogueLog(LogText, LogName, SelectedChoice);
}

bool USweetDreamsDialogueSubsystem::InsertDialogue(const UDialogueData* ToInsert, int32 Index)
{
	if (UDialogueComponent* Component = CurrentDialogueComponent.Get())
	{
		TArray<FSweetDreamsDialogue>& CurrentDialogue = Component->GetDialogueArray();

		if (IsValid(ToInsert))
		{
			if (Index >= 0 && Index <= CurrentDialogue.Num())
			{
				CurrentDialogue.Insert(ToInsert->Dialogues, Index);
			}
			else
			{
				CurrentDialogue.Append(ToInsert->Dialogues);
			}

			return true;
		}
	}

	return false;
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

	FText AnimatedText = FText::FromString(DisplayText);
	OnDialogueAnimating.Broadcast(AnimatedText);

	return AnimatedText;
}
