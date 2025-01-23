// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "LevelSequence.h"
#include "SweetDreamsDialogueManager.generated.h"

class UDialogueWidget;

UENUM(BlueprintType)
enum class EDialogueMode : uint8
{
	DIALOGUE,
	SEQUENCE
};

USTRUCT(BlueprintType)
struct FSweetDreamsDialogue : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	EDialogueMode Mode = EDialogueMode::DIALOGUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::SEQUENCE", EditConditionHides))
	ULevelSequence* DialogueSequence = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	FText DialogueName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true", EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	FText DialogueBody;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true", ForceInlineRow, EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	TMap<int32, FText> Choices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	UTexture2D* DialogueImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera", meta = (ClampMin = -1, EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	int32 CameraID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera", meta = (ClampMin = 0, UIMin = 0, EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	float CameraBlend;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	USoundBase* DialogueAudio;

	FSweetDreamsDialogue()
		: DialogueName(NSLOCTEXT("Dialogue", "DialogueName", "Name")),
		DialogueBody(NSLOCTEXT("Dialogue", "DialogueBody", "Hello, I'm Name.")),
		DialogueImage(nullptr),
		CameraID(-1),
		CameraBlend(0.f),
		DialogueAudio(nullptr),
		Mode(EDialogueMode::DIALOGUE),
		DialogueSequence(nullptr)
	{}
};

USTRUCT(BlueprintType)
struct FSweetDreamsDialogueLog
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	FText DialogueName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	FText DialogueBody;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	FText SelectedChoice;

	FSweetDreamsDialogueLog() 
		: DialogueName(NSLOCTEXT("Dialogue", "DialogueName", "Name")),
		DialogueBody(NSLOCTEXT("Dialogue", "DialogueBody", "Hello, I'm Name.")),
		SelectedChoice(NSLOCTEXT("Dialogue", "DialogueChoice", "This is my choice."))
	{}
	FSweetDreamsDialogueLog(FText NameValue, FText BodyValue, FText ChoiceValue)
		: DialogueName(NameValue),
		DialogueBody(BodyValue),
		SelectedChoice(ChoiceValue)
	{}

	void UpdateChoice(FText NewChoice)
	{
		SelectedChoice = NewChoice;
	}
};

USTRUCT(BlueprintType)
struct FSweetDreamsChoiceDialogues : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	TArray<FSweetDreamsDialogue> ChoiceDialogues;
	FSweetDreamsChoiceDialogues() {}
};

UCLASS()
class SWEETDREAMS_API ASweetDreamsDialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASweetDreamsDialogueManager();

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void StartDialogue(float TransitionDuration = 2.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void UpdateDialogue();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void ApplyChoiceAndContinue(int32 ChoiceIndex = 0);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void EndDialogue();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Sweet Dreams|Core|Dialogue")
	void OnDialogueStarted();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Sweet Dreams|Core|Dialogue")
	void OnDialogueChanged(FSweetDreamsDialogue Dialogue, int32 DialogueID);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Sweet Dreams|Core|Dialogue")
	void OnDialogueEnded();
	//
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Dialogue", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsDialogueManager* GetActiveDialogue(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Dialogue", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsDialogueManager* FindDialogueByName(const UObject* WorldContext, FName Name);
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
	FText GetAnimatedBody() const { return DialogueBodyAnimated; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMulticameraComponent* MulticameraComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	FName DialogueName;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "!bGetDataTableDialogues", EditConditionHides))
	TArray<FSweetDreamsDialogue> Dialogues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "!bGetDataTableChoiceDialogues", EditConditionHides))
	TArray<FSweetDreamsChoiceDialogues> AllChoiceDialogues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "bGetDataTableDialogues", EditConditionHides))
	UDataTable* DialogueTable;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "bGetDataTableChoiceDialogues", EditConditionHides))
	UDataTable* ChoiceDialogueTable;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	bool bGetDataTableDialogues = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	bool bGetDataTableChoiceDialogues = false;
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
	class ALevelSequenceActor* SequenceActor = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue Sequence")
	class ULevelSequencePlayer* SequencePlayer = nullptr;
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void AddDialogueToLog(int32 DialogueID);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void GetDialoguesFromDataTable(UDataTable* Data);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Dialogue")
	void GetChoiceDialoguesFromDataTable(UDataTable* Data);
	UFUNCTION(Category = "Sweet Dreams|Core|Dialogue")
	void StartSequence(FSweetDreamsDialogue Dialogue);
	UFUNCTION(Category = "Sweet Dreams|Core|Dialogue")
	void EndSequence();
	UFUNCTION(Category = "Sweet Dreams|Core|Dialogue")
	void ProcessDialogue(FSweetDreamsDialogue Dialogue);
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
	FText DialogueBodyAnimated = NSLOCTEXT("Dialogue", "DialogueBodyAnimated", "");
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue UI")
	float LetterDisplayElapsed = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue UI")
	FString FullDialogueBody;
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue UI")
	int32 CurrentLetterIndex = 0;
};
