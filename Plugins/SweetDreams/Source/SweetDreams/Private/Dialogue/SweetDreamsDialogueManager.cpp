// Fill out your copyright notice in the Description page of Project Settings.

#include "Dialogue/SweetDreamsDialogueManager.h"
#include "Kismet/GameplayStatics.h"
#include "CineCameraComponent.h"
#include "Player/MulticameraComponent.h"
#include "Player/SweetDreamsHUD.h"
#include "Components/AudioComponent.h"
#include "Dialogue/DialogueWidget.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Dialogue/DialogueData.h"
#include "GameFramework/Character.h"

ASweetDreamsDialogueManager::ASweetDreamsDialogueManager()
{
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* DialogueManager = CreateDefaultSubobject<USceneComponent>(TEXT("Dialogue Manager"));
	DialogueManager->SetupAttachment(RootComponent);

	CameraGroup = CreateDefaultSubobject<USceneComponent>(TEXT("Camera Root"));
	CameraGroup->SetupAttachment(DialogueManager);

	CineCameraComponent = CreateDefaultSubobject<UCineCameraComponent>(TEXT("Camera"));
	CineCameraComponent->SetupAttachment(CameraGroup);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);

	MulticameraComponent = CreateDefaultSubobject<UMulticameraComponent>(TEXT("Multicamera Component"));
	AddOwnedComponent(MulticameraComponent);

	DialogueData = nullptr;
}

void ASweetDreamsDialogueManager::BeginPlay()
{
	GetDialoguesFromData(DialogueData);
	Super::BeginPlay();
}

void ASweetDreamsDialogueManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsAnimating || !bUseAnimatedDialogue) return;
	LetterDisplayElapsed += DeltaTime;
	const int32 LettersToAdvance = FMath::FloorToInt(LetterDisplayElapsed / LetterDisplayRate);
	if (LettersToAdvance > 0)
	{
		if (IsValid(CurrentDialogue.AnimatedLetterAudio))
		{
			UGameplayStatics::PlaySound2D(this, CurrentDialogue.AnimatedLetterAudio);
		}
		CurrentLetterIndex = FMath::Min(CurrentLetterIndex + LettersToAdvance, ProcessedDialogueBody.Len());
		LetterDisplayElapsed = 0.f;
		UpdateAnimatedDialogue();
		if (CurrentLetterIndex >= ProcessedDialogueBody.Len())
		{
			bIsAnimating = false;
		}
	}
}

void ASweetDreamsDialogueManager::StartDialogue(float TransitionDuration)
{
	if (TransitionDuration <= 0.f) TransitionDuration = GetWorld()->GetDeltaSeconds();
	if (!bIsDialogueEnabled || bIsDialogueActive) return;
	if (Dialogues.Num() == 0) return;
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
				DialogueWidget->SetDialogueManager(this);
				SweetDreamsHUD->ShowWidget(DialogueWidget);
				DialogueWidget->OnDialogueStarted();
			}
		}
	}
	bIsDialogueActive = true;
	UpdateDialogue();
	OnDialogueStarted.Broadcast();
}

void ASweetDreamsDialogueManager::UpdateDialogue()
{
	if (bIsSelectingChoices) return;
	if (bUseAnimatedDialogue && bIsAnimating)
	{
		return SkipAnimatedDialogue();
	}
	if (IsValid(CurrentSequencePlayer) && CurrentSequencePlayer->IsPlaying())
	{
		if (!bStopSequenceOnUpdate) return;
		CurrentSequencePlayer->Stop();
	}
	if (CurrentDialogueID >= Dialogues.Num() - 1)
	{
		EndDialogue();
		return;
	}
	CurrentDialogueID = FMath::Clamp(++CurrentDialogueID, 0, Dialogues.Num());
	CurrentDialogue = Dialogues[CurrentDialogueID];
	CallFunctionsFromDialogue(CurrentDialogue);
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

void ASweetDreamsDialogueManager::SkipAnimatedDialogue()
{
	CurrentLetterIndex = ProcessedDialogueBody.Len();
	UpdateAnimatedDialogue();
	bIsAnimating = false;
}

void ASweetDreamsDialogueManager::ProcessDialogue(FSweetDreamsDialogue Dialogue)
{
	FullDialogueBody = CurrentDialogue.DialogueBody.ToString();
	if (bUseAnimatedDialogue)
	{
		AnimatedDialogueBody = FText::GetEmpty();
		ProcessedDialogueBody.Empty();
		for (int32 i = 0; i < FullDialogueBody.Len();)
		{
			if (FullDialogueBody[i] == '<')
			{
				ProcessRichTextTags(i);
			}
			else
			{
				ProcessedDialogueBody.AppendChar(FullDialogueBody[i++]);
			}
		}
		CurrentLetterIndex = 0;
		LetterDisplayElapsed = 0.f;
		bIsAnimating = true;
	}
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
		if (CurrentDialogue.Mode == EDialogueMode::DIALOGUE)
		{
			DialogueWidget->SetCurrentDialogue(CurrentDialogue);
			DialogueWidget->OnUpdatedDialogue(CurrentDialogue, bIsSelectingChoices);
		}
		if (bIsSelectingChoices)
		{
			DialogueWidget->ShowChoices();
		}
		else
		{
			DialogueWidget->HideChoices();
		}
	}
	OnDialogueChanged.Broadcast(CurrentDialogue, CurrentDialogueID);
}

void ASweetDreamsDialogueManager::CallFunctionsFromDialogue(FSweetDreamsDialogue Dialogue)
{
	if (Dialogue.FunctionsToCall.Num() == 0) return;
	for (const FName& FunctionName : Dialogue.FunctionsToCall)
	{
		if (!FunctionName.IsNone())
		{
			UFunction* Function = FindFunction(FunctionName);
			if (IsValid(Function))
			{
				ProcessEvent(Function, nullptr);
			}
		}
	}
}

void ASweetDreamsDialogueManager::ProcessRichTextTags(int32& LetterIndex)
{
	int32 TagEnd = FullDialogueBody.Find(TEXT(">"), ESearchCase::IgnoreCase, ESearchDir::FromStart, LetterIndex);
	if (TagEnd != INDEX_NONE)
	{
		LetterIndex = TagEnd + 1;
	}
}

void ASweetDreamsDialogueManager::UpdateAnimatedDialogue()
{
	FString DisplayText;
	int32 VisibleChars = 0;
	for (int32 i = 0; i < FullDialogueBody.Len(); i++)
	{
		if (FullDialogueBody[i] == '<')
		{
			int32 TagEnd = FullDialogueBody.Find(">", ESearchCase::IgnoreCase, ESearchDir::FromStart, i);
			if (TagEnd != INDEX_NONE)
			{
				DisplayText += FullDialogueBody.Mid(i, TagEnd - i + 1);
				i = TagEnd;
				continue;
			}
		}
		if (VisibleChars < CurrentLetterIndex)
		{
			DisplayText += FullDialogueBody[i];
			VisibleChars++;
		}
	}
	AnimatedDialogueBody = FText::FromString(DisplayText);
	if (IsValid(DialogueWidget))
	{
		DialogueWidget->UpdateAnimatedDialogue(AnimatedDialogueBody);
	}
}

void ASweetDreamsDialogueManager::ApplyChoiceAndContinue(FChoice Choice)
{
	DialogueLog[CurrentDialogueID].UpdateChoice(Choice.ChoiceBody);
	bIsSelectingChoices = false;
	if (DialogueData->ChoiceResults.Contains(Choice.ChoiceResult))
	{
		Dialogues.Append(DialogueData->ChoiceResults[Choice.ChoiceResult].Dialogues);
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
		DialogueWidget->HideSelf();
		DialogueWidget->SetDialogueManager(nullptr);
	}
	OnDialogueEnded.Broadcast();
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
		if (IsValid(Dialogue) && Dialogue->bIsDialogueActive)
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
		if (auto* Dialogue = Cast<ASweetDreamsDialogueManager>(Actor))
		{
			if (Dialogue->DialogueName.IsEqual(Name))
			{
				return Dialogue;
			}
		}
	}
	return nullptr;
}

ASweetDreamsDialogueManager* ASweetDreamsDialogueManager::StartDialogueByName(const UObject* WorldContext, FName Name, float StartTransition)
{
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")) || Name.IsNone())
	{
		return nullptr;
	}
	ASweetDreamsDialogueManager* Dialogue = FindDialogueByName(WorldContext, Name);
	if (IsValid(Dialogue))
	{
		Dialogue->StartDialogue(StartTransition);
	}
	return Dialogue;
}

void ASweetDreamsDialogueManager::UpdateDialogueName(FName NewName)
{
	if (NewName.IsNone()) return;
	DialogueName = NewName;
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

void ASweetDreamsDialogueManager::GetDialoguesFromData(UDialogueData* Data)
{
	if (!IsValid(Data)) return;
	UpdateDialogueName(Data->Name);
	Dialogues.Empty();
	TArray<FSweetDreamsDialogue> DialoguesData = Data->Dialogues;
	for (FSweetDreamsDialogue SingleData : DialoguesData)
	{
		Dialogues.Add(SingleData);
	}
}

void ASweetDreamsDialogueManager::StartSequence(FSweetDreamsDialogue Dialogue)
{
	if (!IsValid(Dialogue.DialogueSequence))
	{
		UpdateDialogue();
		return;
	}
	CurrentSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Dialogue.DialogueSequence, FMovieSceneSequencePlaybackSettings(), CurrentSequenceActor);
	if (!IsValid(CurrentSequencePlayer)) return;
	CurrentSequencePlayer->Play();
	CurrentSequencePlayer->OnFinished.AddDynamic(this, &ASweetDreamsDialogueManager::EndSequence);
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
	CurrentSequencePlayer = nullptr;
	if (IsValid(CurrentSequenceActor))
	{
		CurrentSequenceActor->Destroy();
	}
	CurrentSequenceActor = nullptr;
	UpdateDialogue();
}



