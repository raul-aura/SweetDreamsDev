// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
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
struct SWEETDREAMSDIALOGUE_API FDialogueFunction
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Function")
	FName FunctionName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Function")
	TArray<FString> FunctionParameters;

	FDialogueFunction() {}
};

USTRUCT(BlueprintType)
struct SWEETDREAMSDIALOGUE_API FAnimatedDialogueSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animated Dialogue")
	float LetterDisplayRate = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animated Dialogue")
	TObjectPtr<USoundBase> AnimatedLetterAudio;

	FAnimatedDialogueSettings() {}
};


USTRUCT(BlueprintType)
struct SWEETDREAMSDIALOGUE_API FChoice
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	FText ChoiceBody;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	FName ChoiceResult;

	FChoice() {}
};

USTRUCT(BlueprintType)
struct SWEETDREAMSDIALOGUE_API FSweetDreamsDialogue
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	EDialogueMode Mode = EDialogueMode::DIALOGUE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::SEQUENCE", EditConditionHides))
	TObjectPtr<ULevelSequence> DialogueSequence = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	FText DialogueName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true", EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	FText DialogueBody;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true", ForceInlineRow, EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	TArray<FChoice> Choices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	TObjectPtr<UTexture2D> DialogueImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera", meta = (ClampMin = -1, EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	int32 CameraID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera", meta = (ClampMin = 0, UIMin = 0, EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	float CameraBlend;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue", meta = (EditCondition = "Mode==EDialogueMode::DIALOGUE", EditConditionHides))
	TObjectPtr<USoundBase> DialogueAudio;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	TArray<FDialogueFunction> FunctionsToCall;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animated Dialogue")
	FAnimatedDialogueSettings AnimatedSettings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Parameters")
	TArray<float> CustomFloats;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Parameters")
	TArray<bool> CustomBools;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Parameters")
	TArray<FName> CustomNames;

	FSweetDreamsDialogue()
		: Mode(EDialogueMode::DIALOGUE),
		DialogueSequence(nullptr),
		DialogueName(NSLOCTEXT("Dialogue", "DialogueName", "Name")),
		DialogueBody(NSLOCTEXT("Dialogue", "DialogueBody", "Hello, I'm Name.")),
		DialogueImage(nullptr),
		CameraID(-1),
		CameraBlend(0.f),
		DialogueAudio(nullptr)
	{}
};

USTRUCT(BlueprintType)
struct SWEETDREAMSDIALOGUE_API FChoiceDialogue
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	TArray<FSweetDreamsDialogue> Dialogues;
	FChoiceDialogue() {}
};

USTRUCT(BlueprintType)
struct SWEETDREAMSDIALOGUE_API FSweetDreamsDialogueLog
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
