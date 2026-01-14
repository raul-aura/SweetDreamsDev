// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/SweetDreamsWidget.h"
#include "DialogueStructs.h"
#include "DialogueWidget.generated.h"

UCLASS()
class SWEETDREAMSDIALOGUE_API UDialogueWidget : public USweetDreamsWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void SetCurrentDialogue(FSweetDreamsDialogue Dialogue);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void GoToNextDialogue();
	void UpdateAnimatedDialogue(FText AnimatedText);

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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void OnAnimatedDialogueStarted();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void OnAnimatedDialogueFinished();
	
	//
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void ShowChoices();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void HideChoices();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void SelectChoice(FChoice Choice);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void SelectChoiceByIndex(int32 Choice);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	FSweetDreamsDialogue CurrentDialogue = FSweetDreamsDialogue();
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	TArray<FChoice> CurrentChoices;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	FText AnimatedDialogueBody = FText(); 
};
