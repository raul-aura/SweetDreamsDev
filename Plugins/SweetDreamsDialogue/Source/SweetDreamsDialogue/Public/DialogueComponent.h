

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueDelegates.h"
#include "DialogueComponent.generated.h"

class UDialogueData;
class USweetDreamsDialogueSubsystem;

// An actor component that is in charge of executing dialogues and communicating with the subsystem.
UCLASS( ClassGroup=("SweetDreams"), meta=(BlueprintSpawnableComponent) )
class SWEETDREAMSDIALOGUE_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UDialogueComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartDialogue(UDialogueData* Dialogue);
	void ProcessDialogue();
	void SelectChoice(FGameplayTag Choice);
	void EndDialogue();

	void SkipAnimatedDialogue();

	bool CanAdvanceDialogue();
	bool ShouldDialogueEnd() const;

	TArray<FSweetDreamsDialogue>& GetDialogueArray();
	const FSweetDreamsDialogue& GetCurrentDialogue() const;
	int32 GetCurrentDialogueIndex() const;
	bool IsInExecution() const;
	FText GetCurrentAnimatedText() const;
	bool IsAnimatingDialogue() const;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue Component")
	FOnDialogueDelegate OnDialogueStarted;
	UPROPERTY(BlueprintAssignable, Category = "Dialogue Component")
	FOnDialogueDelegate OnDialogueEnded;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Component")
	bool bDialogueInExecution = false;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Component")
	bool bIsSelectingChoices = false;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Component")
	TObjectPtr<UDialogueData> DialogueData;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Component")
	TArray<FSweetDreamsDialogue> Dialogues;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Component")
	TArray<FSweetDreamsDialogueLog> DialogueLog;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Component")
	FSweetDreamsDialogue CurrentDialogue;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Component")
	int32 CurrentDialogueID = -1;

	// Animation
	bool bIsAnimating = false;
	float LetterDisplayElapsed = 0.f;
	int32 CurrentLetterIndex = 0;
	FString TaglessDialogueBody;
	FText AnimatedDialogueBody;
	FAnimatedDialogueSettings CurrentAnimatedSettings;

private:

	void StartAnimatedDialogue(const FSweetDreamsDialogue& Dialogue);
	void BuildAnimatedDialogue();

	USweetDreamsDialogueSubsystem* GetDialogueSubsystem() const;
};
