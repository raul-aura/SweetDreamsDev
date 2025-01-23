// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsWidget.h"
#include "SweetDreamsDialogueManager.h"
#include "DialogueWidget.generated.h"

UCLASS()
class SWEETDREAMS_API UDialogueWidget : public USweetDreamsWidget
{
	GENERATED_BODY()

	friend ASweetDreamsDialogueManager;
public:

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void SetCurrentDialogue(FSweetDreamsDialogue Dialogue);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void GoToNextDialogue();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void OnDialogueStarted();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void OnUpdatedDialogue(FSweetDreamsDialogue Dialogue, bool bHasChoices);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void OnDialogueEnded();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void OnSequenceStarted();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void OnSequenceEnded();
	//
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void ShowChoices();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void HideChoices();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void SelectChoice(int32 ChoiceSelected);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	FSweetDreamsDialogue CurrentDialogue = FSweetDreamsDialogue();
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	TMap<int32, FText> CurrentChoices;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	FText AnimatedDialogueBody = FText();
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	ASweetDreamsDialogueManager* DialogueManager = nullptr;
};
