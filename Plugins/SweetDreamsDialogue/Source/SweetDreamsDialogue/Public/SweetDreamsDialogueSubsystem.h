

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueDelegates.h"
#include "SweetDreamsDialogueSubsystem.generated.h"

class UDialogueData;

UCLASS(Category = "SweetDreams|Dialogue")
class SWEETDREAMSDIALOGUE_API USweetDreamsDialogueSubsystem final : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	USweetDreamsDialogueSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	static USweetDreamsDialogueSubsystem* Get(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void StartDialogue(UDialogueComponent* Component, UDialogueData* Dialogue);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void UpdateDialogue();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void EndDialogue();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void SelectChoiceAndUpdate(FGameplayTag Choice);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void SkipAnimatedDialogue();

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	bool IsAnyDialogueInExecution() const;

	// Converts a dialogue struct, and selected choice, if present, to a dialogue log struct.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	FSweetDreamsDialogueLog ConvertDialogueToLog(const FSweetDreamsDialogue& Dialogue, const FText SelectedChoice) const;

	// Inserts the dialogue from specified data to the current dialogue, if available. The index is relative to the current dialogue index to prevent inserting text that won't be shown to the player.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	bool InsertDialogue(const UDialogueData* ToInsert, int32 Index = -1);
	
	// Converts the body of the received dialogue into a string without tags.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	FString GetTaglessAnimatedDialogue(const FSweetDreamsDialogue& Dialogue) const;
	
	// Converts the body of the received dialogue into a proper animated text, ignoring tags and respecting the letter index.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	FText GetAnimatedDialogue(const FSweetDreamsDialogue& Dialogue, const int32& LetterIndex) const;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue Subsystem")
	FOnDialogueDelegate OnDialogueStarted;
	UPROPERTY(BlueprintAssignable, Category = "Dialogue Subsystem")
	FOnDialogueDelegate OnDialogueEnded;
	UPROPERTY(BlueprintAssignable, Category = "Dialogue Subsystem")
	FOnDialogueUpdated OnDialogueUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Dialogue Subsystem")
	FOnDialogueChoice OnDialogueChoices;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue Subsystem")
	FOnDialogueSequence OnDialogueSequence;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue Subsystem")
	FOnDialogueAnimatedText OnDialogueAnimating;
	UPROPERTY(BlueprintAssignable, Category = "Dialogue Subsystem")
	FOnDialogueAnimatedText OnDialogueAnimationFinished;

private:

	TWeakObjectPtr<UDialogueComponent> CurrentDialogueComponent;
};
