// Fill out your copyright notice in the Description page of Project Settings.

#include "Dialogue/DialogueWidget.h"
#include "Dialogue/SweetDreamsDialogueManager.h"

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

void UDialogueWidget::SetDialogueManager(ASweetDreamsDialogueManager* NewManager)
{
	DialogueManager = NewManager;
}

void UDialogueWidget::GoToNextDialogue()
{
	if (IsValid(DialogueManager))
	{
		DialogueManager->UpdateDialogue();
	}
}

void UDialogueWidget::UpdateAnimatedDialogue(FText AnimatedText)
{
	AnimatedDialogueBody = AnimatedText;
}

void UDialogueWidget::SelectChoice(FChoice ChoiceSelected)
{
	if (IsValid(DialogueManager))
	{
		DialogueManager->ApplyChoiceAndContinue(ChoiceSelected);
	}
}
