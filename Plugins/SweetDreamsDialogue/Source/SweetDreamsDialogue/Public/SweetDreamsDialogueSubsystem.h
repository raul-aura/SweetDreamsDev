

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueDelegates.h"
#include "SweetDreamsDialogueSubsystem.generated.h"

class UDialogueData;

UCLASS(Category = "SweetDreams|Dialogue")
class SWEETDREAMSDIALOGUE_API USweetDreamsDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USweetDreamsDialogueSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Converts a dialogue struct, and selected choice, if present, to a dialogue log struct.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	FSweetDreamsDialogueLog ConvertDialogueToLog(const FSweetDreamsDialogue& Dialogue, const FChoice SelectedChoice) const;
	// Inserts the dialogue from specified data to the current dialogue. The index is relative to the current dialogue index to prevent inserting text that won't be shown to the player.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void InsertDialogue(TArray<FSweetDreamsDialogue>& CurrentDialogue, const UDialogueData* ToInsert, int32 Index = -1);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void SelectChoice(const FChoice& Choice);

	// Converts the body of the received dialogue into a string without tags.
	FString GetTaglessAnimatedDialogue(const FSweetDreamsDialogue& Dialogue) const;
	// Converts the body of the received dialogue into a proper animated text, ignoring tags and respecting the letter index.
	FText GetAnimatedDialogue(const FSweetDreamsDialogue& Dialogue, const int32& LetterIndex) const;
};
