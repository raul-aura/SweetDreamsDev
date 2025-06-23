// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dialogue/DialogueStructs.h"
#include "SweetDreamsDialogueManager.generated.h"

class UDialogueWidget;
class UDialogueData;
class UCineCameraComponent;
class UMulticameraComponent;

UCLASS()
class SWEETDREAMS_API ASweetDreamsDialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASweetDreamsDialogueManager();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueStarted);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueChanged, FSweetDreamsDialogue, Dialogue, int32, DialogueID);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void StartDialogue(float TransitionDuration = 2.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void UpdateDialogue();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void SkipAnimatedDialogue();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void ApplyChoiceAndContinue(FChoice Choice);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void EndDialogue();
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Dialogue")
	FOnDialogueStarted OnDialogueStarted;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Dialogue")
	FOnDialogueChanged OnDialogueChanged;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Dialogue")
	FOnDialogueEnded OnDialogueEnded;
	//
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Dialogue", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsDialogueManager* GetActiveDialogue(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Dialogue", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsDialogueManager* FindDialogueByName(const UObject* WorldContext, FName Name);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsDialogueManager* StartDialogueByName(const UObject* WorldContext, FName Name, float StartTransition = 2.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void UpdateDialogueName(FName NewName);
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	int32 GetCurrentDialogueID() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	FSweetDreamsDialogue GetCurrentDialogue() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	TArray<FSweetDreamsDialogueLog> GetDialogueLog() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	bool GetIsDialogueActive() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	FText GetAnimatedBody() const { return AnimatedDialogueBody; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCineCameraComponent* CineCameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMulticameraComponent* MulticameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* CameraGroup;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogues")
	UDialogueData* DialogueData;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FName DialogueName = NAME_None;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	TArray<FSweetDreamsDialogue> Dialogues;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	bool bStopSequenceOnUpdate = false;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	bool bIsDialogueActive = false;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	bool bIsSelectingChoices = false;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue Log")
	TArray<FSweetDreamsDialogueLog> DialogueLog;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	FSweetDreamsDialogue CurrentDialogue;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	int32 CurrentDialogueID = -1;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	APawn* OriginalPawn = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue Sequence")
	class ALevelSequenceActor* CurrentSequenceActor = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue Sequence")
	class ULevelSequencePlayer* CurrentSequencePlayer = nullptr;
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void AddDialogueToLog(int32 DialogueID);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void GetDialoguesFromData(UDialogueData* Data);
	UFUNCTION(Category = "Sweet Dreams|Core|Dialogue")
	void StartSequence(FSweetDreamsDialogue Dialogue);
	UFUNCTION(Category = "Sweet Dreams|Core|Dialogue")
	void EndSequence();
	UFUNCTION(Category = "Sweet Dreams|Core|Dialogue")
	void ProcessDialogue(FSweetDreamsDialogue Dialogue);
	UFUNCTION(Category = "Sweet Dreams|Core|Dialogue")
	void CallFunctionsFromDialogue(FSweetDreamsDialogue Dialogue);
	UFUNCTION(Category = "Sweet Dreams|Core|Dialogue")
	void ProcessRichTextTags(int32& LetterIndex);
	UFUNCTION(Category = "Sweet Dreams|Core|Dialogue")
	void UpdateAnimatedDialogue();
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Settings")
	bool bCanRepeatDialogue = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Settings")
	bool bPossessThis = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Settings")
	bool bHideCharacter = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Settings")
	bool bIsDialogueEnabled = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Settings")
	float EndTransitionDuration = 2.0f;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue UI")
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue UI")
	UDialogueWidget* DialogueWidget = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue UI")
	bool bUseAnimatedDialogue = true;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue UI")
	bool bIsAnimating = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue UI")
	float LetterDisplayRate = 0.1f;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue UI")
	FText AnimatedDialogueBody;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue UI")
	float LetterDisplayElapsed = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue UI")
	FString FullDialogueBody;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue UI")
	FString ProcessedDialogueBody;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue UI")
	int32 CurrentLetterIndex = 0;
};
