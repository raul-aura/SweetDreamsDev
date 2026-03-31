

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

	UPROPERTY(BlueprintAssignable)
	FOnDialogueEvent OnDialogueStarted;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueEvent OnDialogueEnded;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueUpdated OnDialogueUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueFunction OnDialogueCustomFunction;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueAnimation OnDialogueAnimating;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueAnimation OnDialogueAnimationFinished;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueChoice OnDialogueChoices;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueEvent OnDialogueNoChoices;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueAudio OnDialogueAudio;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueSequence OnDialogueSequence;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueUpdated OnCustomDialogueMode;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	UDialogueData* GetDialogueData() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void SetDialogueData(UDialogueData* InData);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	TArray<FSweetDreamsDialogue> GetDialogues() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	TArray<FSweetDreamsDialogueLog> GetDialogueLog() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	FSweetDreamsDialogue GetCurrentDialogue(int32& Index) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	int32 GetCurrentDialogueID() const;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	bool IsUsingAnimatedDialogue() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Dialogue")
	void SetUseAnimatedDialogue(bool bInUseAnimatedDialogue);

protected:
	void ProcessDialogue(FSweetDreamsDialogue Dialogue, int32 Index);
	void AddDialogueToLog(FSweetDreamsDialogue Dialogue);
	void CallCustomFunctions(FSweetDreamsDialogue Dialogue, int32 Index);
	void ResetDialogueData();
	void EndDialogue();

	void SkipRichTextTags(int32& LetterIndex);
	void ProcessAnimatedDialogue(FSweetDreamsDialogue Dialogue);
	bool CanAdvanceLetters();
	void BuildAnimatedDialogue();

	TObjectPtr<UDialogueData> DialogueData = nullptr;
	TArray<FSweetDreamsDialogue> Dialogues;

	TArray<FSweetDreamsDialogueLog> DialogueLog;
	FSweetDreamsDialogue CurrentDialogue;
	int32 CurrentDialogueID = -1;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bIsDialogueActive = false;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bIsSelectingChoices = false;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bIsAnimating = false;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bUseAnimatedDialogue = true;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FAnimatedDialogueSettings CurrentAnimatedSettings;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FText AnimatedDialogueBody;
	FString FullDialogueBody;
	FString TaglessDialogueBody;
	int32 CurrentLetterIndex = 0;
	float LetterDisplayElapsed = 0.f;
};
