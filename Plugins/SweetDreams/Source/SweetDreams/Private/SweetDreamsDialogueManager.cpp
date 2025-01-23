// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsDialogueManager.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "MulticameraComponent.h"
#include "SweetDreamsHUD.h"
#include "Components/AudioComponent.h"
#include "DialogueWidget.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "GameFramework/Character.h"

ASweetDreamsDialogueManager::ASweetDreamsDialogueManager()
{
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* DialogueManager = CreateDefaultSubobject<USceneComponent>(TEXT("Dialogue Manager"));
	DialogueManager->SetupAttachment(RootComponent);

	USceneComponent* CameraGroup = CreateDefaultSubobject<USceneComponent>(TEXT("Camera Root"));
	CameraGroup->SetupAttachment(DialogueManager);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Primary Camera"));
	CameraComponent->SetupAttachment(CameraGroup);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);

	MulticameraComponent = CreateDefaultSubobject<UMulticameraComponent>(TEXT("Multicamera Component"));
	AddOwnedComponent(MulticameraComponent);
}

void ASweetDreamsDialogueManager::BeginPlay()
{
	if (bGetDataTableDialogues) GetDialoguesFromDataTable(DialogueTable);
	if (bGetDataTableChoiceDialogues) GetChoiceDialoguesFromDataTable(ChoiceDialogueTable);
	Super::BeginPlay();
}

void ASweetDreamsDialogueManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsDialogueActive && bUseAnimatedDialogue)
	{
		if (IsValid(DialogueWidget))
		{
			DialogueWidget->AnimatedDialogueBody = DialogueBodyAnimated;
		}
		if (!CurrentDialogue.DialogueBody.IsEmpty() && !CurrentDialogue.DialogueBody.EqualTo(DialogueBodyAnimated))
		{
			bIsAnimating = true;
			LetterDisplayElapsed += DeltaTime;
			if (LetterDisplayElapsed >= LetterDisplayRate && CurrentLetterIndex < FullDialogueBody.Len())
			{
				FString AnimatedText = FullDialogueBody.Left(CurrentLetterIndex + 1);
				DialogueBodyAnimated = FText::FromString(AnimatedText);
				++CurrentLetterIndex;
				LetterDisplayElapsed = 0.f;
			}
		}
		else
		{
			bIsAnimating = false;
		}
	}
}

void ASweetDreamsDialogueManager::StartDialogue(float TransitionDuration)
{
	if (TransitionDuration <= 0.f) TransitionDuration = GetWorld()->GetDeltaSeconds();
	if (Dialogues.Num() == 0 || !bIsDialogueEnabled || bIsDialogueActive) return;
	if (bHideCharacter)
	{
		if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			Character->SetActorHiddenInGame(true);
		}
	}
	if (bPossessThis)
	{
		if (APlayerController* Player = GetWorld()->GetFirstPlayerController())
		{
			OriginalPawn = Player->GetPawn();
			Player->SetViewTargetWithBlend(this, TransitionDuration);
		}
	}
	if (IsValid(DialogueWidgetClass))
	{
		if (ASweetDreamsHUD* SweetDreamsHUD = Cast<ASweetDreamsHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			DialogueWidget = Cast<UDialogueWidget>(ASweetDreamsHUD::FindWidgetByClass(DialogueWidgetClass));
			if (IsValid(DialogueWidget))
			{
				DialogueWidget->DialogueManager = this;
				SweetDreamsHUD->ShowWidget(DialogueWidget);
				DialogueWidget->OnDialogueStarted();
			}
		}
	}
	bIsDialogueActive = true;
	UpdateDialogue();
	OnDialogueStarted();
}

void ASweetDreamsDialogueManager::UpdateDialogue()
{
	if (bIsSelectingChoices) return;
	if (bUseAnimatedDialogue && bIsAnimating)
	{
		DialogueBodyAnimated = CurrentDialogue.DialogueBody;
		return;
	}
	if (CurrentDialogueID >= Dialogues.Num() - 1)
	{
		EndDialogue();
		return;
	}
	CurrentDialogueID = FMath::Clamp(++CurrentDialogueID, 0, Dialogues.Num());
	CurrentDialogue = Dialogues[CurrentDialogueID];
	EDialogueMode CurrentMode = CurrentDialogue.Mode;
	switch (CurrentMode)
	{
	case EDialogueMode::DIALOGUE:
		ProcessDialogue(CurrentDialogue);
		break;
	case EDialogueMode::SEQUENCE:
		StartSequence(CurrentDialogue);
		break;
	default:
		break;
	}
}

void ASweetDreamsDialogueManager::ApplyChoiceAndContinue(int32 ChoiceIndex)
{
	if (!AllChoiceDialogues.IsValidIndex(ChoiceIndex)) return;
	DialogueLog[CurrentDialogueID].UpdateChoice(CurrentDialogue.Choices[ChoiceIndex]);
	bIsSelectingChoices = false;
	TArray<FSweetDreamsDialogue> SelectedChoiceDialogues = AllChoiceDialogues[ChoiceIndex].ChoiceDialogues;
	int32 InsertIndex = FMath::Min(CurrentDialogueID + 1, Dialogues.Num());
	for (int32 i = 0; i < SelectedChoiceDialogues.Num(); ++i)
	{
		Dialogues.Insert(SelectedChoiceDialogues[i], InsertIndex + i);
	}
	UpdateDialogue();
}

void ASweetDreamsDialogueManager::EndDialogue()
{
	float TransitionDuration = EndTransitionDuration;
	if (TransitionDuration <= 0.f) TransitionDuration = GetWorld()->GetDeltaSeconds();
	bIsDialogueActive = false;
	CurrentDialogue = FSweetDreamsDialogue();
	DialogueLog.Empty();
	CurrentDialogueID = -1;
	if (bPossessThis)
	{
		if (APlayerController* Player = GetWorld()->GetFirstPlayerController())
		{
			Player->SetViewTargetWithBlend(OriginalPawn, TransitionDuration);
		}
	}
	if (!bCanRepeatDialogue)
	{
		bIsDialogueEnabled = false;
	}
	if (bHideCharacter)
	{
		if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			Character->SetActorHiddenInGame(false);
		}
	}
	if (IsValid(DialogueWidget))
	{
		DialogueWidget->OnDialogueEnded();
		DialogueWidget->DialogueManager = nullptr;
	}
	OnDialogueEnded();
}

ASweetDreamsDialogueManager* ASweetDreamsDialogueManager::GetActiveDialogue(const UObject* WorldContext)
{
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	TArray<AActor*> Dialogues;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASweetDreamsDialogueManager::StaticClass(), Dialogues);
	for (AActor* Actor : Dialogues)
	{
		ASweetDreamsDialogueManager* Dialogue = Cast<ASweetDreamsDialogueManager>(Actor);
		if (Dialogue && Dialogue->bIsDialogueActive)
		{
			return Dialogue;
		}
	}
	return nullptr;
}

ASweetDreamsDialogueManager* ASweetDreamsDialogueManager::FindDialogueByName(const UObject* WorldContext, FName Name)
{
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")) || Name.IsNone())
	{
		return nullptr;
	}
	TArray<AActor*> Dialogues;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASweetDreamsDialogueManager::StaticClass(), Dialogues);
	for (AActor* Actor : Dialogues)
	{
		ASweetDreamsDialogueManager* Dialogue = Cast<ASweetDreamsDialogueManager>(Actor);
		if (Dialogue && Dialogue->DialogueName.IsEqual(Name))
		{
			return Dialogue;
		}
	}
	return nullptr;
}

int32 ASweetDreamsDialogueManager::GetCurrentDialogueID() const
{
	return CurrentDialogueID;
}

FSweetDreamsDialogue ASweetDreamsDialogueManager::GetCurrentDialogue() const
{
	return CurrentDialogue;
}

TArray<FSweetDreamsDialogueLog> ASweetDreamsDialogueManager::GetDialogueLog() const
{
	return DialogueLog;
}

bool ASweetDreamsDialogueManager::GetIsDialogueActive() const
{
	return bIsDialogueActive;
}

void ASweetDreamsDialogueManager::AddDialogueToLog(int32 DialogueID)
{
	FText LogText = Dialogues[DialogueID].DialogueBody;
	FText LogName = Dialogues[DialogueID].DialogueName;
	FSweetDreamsDialogueLog NewLog = FSweetDreamsDialogueLog(LogText, LogName, FText());
	DialogueLog.Add(NewLog);
}

void ASweetDreamsDialogueManager::GetDialoguesFromDataTable(UDataTable* Data)
{
	if (!IsValid(Data)) return;
	TArray<FSweetDreamsDialogue*> LoadedDialogues;
	Data->GetAllRows<FSweetDreamsDialogue>(TEXT("Populate Dialogues"), LoadedDialogues);
	Dialogues.Empty(); 
	for (FSweetDreamsDialogue* Row : LoadedDialogues)
	{
		if (Row)
		{
			Dialogues.Add(*Row);
		}
	}
}

void ASweetDreamsDialogueManager::GetChoiceDialoguesFromDataTable(UDataTable* Data)
{
	if (!IsValid(Data)) return;
	TArray<FSweetDreamsChoiceDialogues*> LoadedChoiceDialogues;
	Data->GetAllRows<FSweetDreamsChoiceDialogues>(TEXT("Populate Dialogues"), LoadedChoiceDialogues);
	AllChoiceDialogues.Empty();
	for (FSweetDreamsChoiceDialogues* Row : LoadedChoiceDialogues)
	{
		if (Row)
		{
			AllChoiceDialogues.Add(*Row);
		}
	}
}

void ASweetDreamsDialogueManager::StartSequence(FSweetDreamsDialogue Dialogue)
{
	if (!IsValid(Dialogue.DialogueSequence))
	{
		UpdateDialogue();
		return;
	}
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Dialogue.DialogueSequence, FMovieSceneSequencePlaybackSettings(), SequenceActor);
	if (!IsValid(SequencePlayer)) return;
	SequencePlayer->Play();
	SequencePlayer->OnFinished.AddDynamic(this, &ASweetDreamsDialogueManager::EndSequence);
	if (IsValid(DialogueWidget))
	{
		DialogueWidget->OnSequenceStarted();
	}
}

void ASweetDreamsDialogueManager::EndSequence()
{
	if (IsValid(DialogueWidget))
	{
		DialogueWidget->OnSequenceEnded();
	}
	SequencePlayer = nullptr;
	if (IsValid(SequenceActor))
	{
		SequenceActor->Destroy();
		SequenceActor = nullptr;
	}
	UpdateDialogue();
}

void ASweetDreamsDialogueManager::ProcessDialogue(FSweetDreamsDialogue Dialogue)
{
	FullDialogueBody = CurrentDialogue.DialogueBody.ToString();
	CurrentLetterIndex = 0;
	LetterDisplayElapsed = 0.f;
	AddDialogueToLog(CurrentDialogueID);
	if (CurrentDialogue.DialogueAudio)
	{
		AudioComponent->SetSound(CurrentDialogue.DialogueAudio);
		AudioComponent->Play();
	}
	if (bPossessThis && CurrentDialogue.CameraID >= 0)
	{
		MulticameraComponent->SetNewCameraView(CurrentDialogue.CameraID, CurrentDialogue.CameraBlend);
	}
	bIsSelectingChoices = CurrentDialogue.Choices.Num() > 0;
	if (IsValid(DialogueWidget))
	{
		DialogueWidget->SetCurrentDialogue(CurrentDialogue);
		if (bIsSelectingChoices)
		{
			DialogueWidget->ShowChoices();
		}
		else
		{
			DialogueWidget->HideChoices();
		}
	}
	OnDialogueChanged(CurrentDialogue, CurrentDialogueID);
}



