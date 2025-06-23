// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Dialogue/DialogueStructs.h"
#include "DialogueData.generated.h"

UCLASS()
class SWEETDREAMS_API UDialogueData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue Data")
	FName Name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue Data")
	TArray<FSweetDreamsDialogue> Dialogues;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue Data")
	TMap<FName, FChoiceDialogue> ChoiceResults;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void GetNameAndBody(int32 DialogueID, UPARAM(ref) FText& DialogueName, UPARAM(ref) FText& DialogueBody) const;
};

