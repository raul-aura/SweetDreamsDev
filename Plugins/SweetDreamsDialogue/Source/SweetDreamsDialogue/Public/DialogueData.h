// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueStructs.h"
#include "DialogueData.generated.h"

UCLASS(BlueprintType)
class SWEETDREAMSDIALOGUE_API UDialogueData : public UDataAsset
{
	GENERATED_BODY()

public:

	// Unique identifier for this dialogue data.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue Data")
	FGameplayTag Identifier;

	// All the texts of dialogues present in this data.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue Data")
	TArray<FSweetDreamsDialogue> Dialogues;

	// When selecting a choice, the dialogue will receive a Gameplay Tag of the selected choice, and will try to match with the tags available below to insert/append the texts of dialogue into the current dialogue.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue Data")
	TMap<FGameplayTag, FChoiceDialogue> ChoiceResults;

	// Helper function to get the speaker name and body of text of the dialogue of index, if valid.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void GetNameAndBody(int32 DialogueID, FText& DialogueName, FText& DialogueBody) const;
};

