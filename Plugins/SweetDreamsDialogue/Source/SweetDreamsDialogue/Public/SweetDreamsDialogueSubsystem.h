

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueStructs.h"
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

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void StartDialogue(UDialogueData* Dialogue);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void UpdateDialogue();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void UpdateAnimatedDialogue(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void SkipAnimatedDialogue();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void SelectChoiceAndUpdate(FChoice Choice);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void InsertDialogue(UDialogueData* Dialogue, int32 Index = -1);

protected:
	void ProcessDialogue(FSweetDreamsDialogue Dialogue);
	void AddDialogueToLog(FSweetDreamsDialogue Dialogue);
	void ResetDialogueData();
	void EndDialogue();

	void SkipRichTextTags(int32& LetterIndex);
	void ProcessAnimatedDialogue();
	bool CanAdvanceLetters();
	void BuildAnimatedDialogue();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogues")
	TObjectPtr<UDialogueData> DialogueData = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	TArray<FSweetDreamsDialogue> Dialogues;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Log")
	TArray<FSweetDreamsDialogueLog> DialogueLog;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FSweetDreamsDialogue CurrentDialogue;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	int32 CurrentDialogueID = -1;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bIsDialogueActive = false;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bIsSelectingChoices = false;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bIsAnimating = false;

	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	bool bUseAnimatedDialogue = true;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	float LetterDisplayRate = 0.1f;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	float LetterDisplayElapsed = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FText AnimatedDialogueBody;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FString FullDialogueBody;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FString TaglessDialogueBody;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	int32 CurrentLetterIndex = 0;
};
