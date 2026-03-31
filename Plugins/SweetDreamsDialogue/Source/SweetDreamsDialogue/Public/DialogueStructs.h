// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DialogueStructs.generated.h"

class ULevelSequence;

UENUM(BlueprintType)
enum class EDialogueMode : uint8
{
	DIALOGUE,
	SEQUENCE,
	CUSTOM
};

USTRUCT(BlueprintType)
struct SWEETDREAMSDIALOGUE_API FAnimatedDialogueSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animated Dialogue")
	float LetterDisplayRate = 0.1f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animated Dialogue")
	TObjectPtr<USoundBase> AnimatedLetterAudio;

	FAnimatedDialogueSettings() {}
};


USTRUCT(BlueprintType)
struct SWEETDREAMSDIALOGUE_API FChoice
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue")
	FText Body;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue")
	FName Result;

	FChoice() {}
};

USTRUCT(BlueprintType)
struct SWEETDREAMSDIALOGUE_API FSweetDreamsDialogue
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue")
	EDialogueMode Mode = EDialogueMode::DIALOGUE;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::SEQUENCE", EditConditionHides))
	TObjectPtr<ULevelSequence> Sequence = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	FText SpeakerName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true", EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	FText Body;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true", ForceInlineRow, EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	TArray<FChoice> Choices;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	TObjectPtr<UTexture2D> SpeakerImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera", meta = (ClampMin = -1, EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	int32 CameraID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera", meta = (ClampMin = 0, UIMin = 0, EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	float CameraBlend;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	TObjectPtr<USoundBase> Audio;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animated Dialogue")
	FAnimatedDialogueSettings AnimatedSettings;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Extra")
	FGameplayTagContainer Tags;

	FSweetDreamsDialogue()
		: Mode(EDialogueMode::DIALOGUE),
		Sequence(nullptr),
		SpeakerName(NSLOCTEXT("Dialogue", "DialogueName", "Name")),
		Body(NSLOCTEXT("Dialogue", "DialogueBody", "Hello, I'm Name.")),
		SpeakerImage(nullptr),
		CameraID(-1),
		CameraBlend(0.f),
		Audio(nullptr)
	{}
};

USTRUCT(BlueprintType)
struct SWEETDREAMSDIALOGUE_API FChoiceDialogue
{
	GENERATED_BODY()

public:

	// Which index, relative to the current dialogue index, should the dialogues of this choice result be inserted.
	// If -1, the dialogues will be appended to the end of the current dialogue.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue")
	int32 IndexToInsert = -1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue")
	TArray<FSweetDreamsDialogue> Dialogues;

	FChoiceDialogue() {}
};

USTRUCT(BlueprintType)
struct SWEETDREAMSDIALOGUE_API FSweetDreamsDialogueLog
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue")
	FText SpeakerName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	FText Body;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue")
	FText SelectedChoice;

	FSweetDreamsDialogueLog()
		: SpeakerName(NSLOCTEXT("Dialogue", "DialogueName", "Name")),
		Body(NSLOCTEXT("Dialogue", "DialogueBody", "Hello, I'm Name.")),
		SelectedChoice(NSLOCTEXT("Dialogue", "DialogueChoice", "This is my choice."))
	{}
	FSweetDreamsDialogueLog(FText NameValue, FText BodyValue, FText ChoiceValue)
		: SpeakerName(NameValue),
		Body(BodyValue),
		SelectedChoice(ChoiceValue)
	{}
};
