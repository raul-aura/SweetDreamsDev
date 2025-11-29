// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueData.h"

void UDialogueData::GetNameAndBody(int32 DialogueID, UPARAM(ref) FText& DialogueName, UPARAM(ref) FText& DialogueBody) const
{
	if (Dialogues.IsValidIndex(DialogueID))
	{
		DialogueName = Dialogues[DialogueID].DialogueName;
		DialogueBody = Dialogues[DialogueID].DialogueBody;
	}
}
