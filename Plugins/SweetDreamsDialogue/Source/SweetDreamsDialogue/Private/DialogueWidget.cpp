// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueWidget.h"

void UDialogueWidget::SetCurrentDialogue(FSweetDreamsDialogue Dialogue)
{
	CurrentDialogue = Dialogue;
	bool bChoices = CurrentDialogue.Choices.Num() > 0;
	if (bChoices)
	{
		CurrentChoices = CurrentDialogue.Choices;
	}
	OnUpdatedDialogue(CurrentDialogue, bChoices);
}

void UDialogueWidget::GoToNextDialogue()
{

}

void UDialogueWidget::UpdateAnimatedDialogue(FText AnimatedText)
{
	AnimatedDialogueBody = AnimatedText;
}

void UDialogueWidget::SelectChoice(FChoice Choice)
{

}

void UDialogueWidget::SelectChoiceByIndex(int32 Choice)
{
	if (CurrentChoices.Num() > 0 && CurrentChoices.IsValidIndex(Choice))
	{
		FChoice ChoiceStruct = CurrentDialogue.Choices[Choice];
		SelectChoice(ChoiceStruct);
	}
}
