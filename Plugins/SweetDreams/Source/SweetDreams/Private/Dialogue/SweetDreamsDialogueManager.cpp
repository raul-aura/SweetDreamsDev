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
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

ASweetDreamsDialogueManager::ASweetDreamsDialogueManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
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

void ASweetDreamsDialogueManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASweetDreamsDialogueManager, CurrentDialogueID);
	DOREPLIFETIME(ASweetDreamsDialogueManager, DialogueData);
}

void ASweetDreamsDialogueManager::BeginPlay()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Updating dialogue data."));
		GetDialoguesFromData(DialogueData);
	}
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

void ASweetDreamsDialogueManager::StartDialogue(float ViewBlend)
{
	if (!bIsDialogueEnabled || bIsDialogueActive || Dialogues.Num() == 0) return;
	if (ViewBlend <= 0.f) ViewBlend = GetWorld()->GetDeltaSeconds();
	MulticastStartDialogue(ViewBlend);
}

void ASweetDreamsDialogueManager::MulticastStartDialogue_Implementation(const float& ViewBlend)
{
	if (bHideCharacter)
	{
		ToggleCharacterVisibility(false);
	}
	if (bPossessThis)
	{
		SetViewTarget(this, ViewBlend);
	}
	CreateShowWidget();
	bIsDialogueActive = true;
	if (HasAuthority())
	{
		UpdateDialogue();
	}
	OnDialogueStarted.Broadcast();
}

void ASweetDreamsDialogueManager::UpdateDialogue()
{
	if (bIsSelectingChoices) return;
	MulticastUpdateDialogue();
}

void ASweetDreamsDialogueManager::MulticastUpdateDialogue_Implementation()
{
	if (bUseAnimatedDialogue && bIsAnimating)
	{
		SkipAnimatedDialogue();
		return;
	}
	if (IsValid(CurrentSequencePlayer) && CurrentSequencePlayer->IsPlaying())
	{
		if (!bStopSequenceOnUpdate) return; // will ignore trying to update if sequence is still playing
		CurrentSequencePlayer->Stop();
	}
	if (CurrentDialogueID >= Dialogues.Num() - 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("ID greater then dialogue length, ENDING."));
		EndDialogue();
		return;
	}
	CurrentDialogueID = FMath::Clamp(++CurrentDialogueID, 0, Dialogues.Num());
	OnRep_CurrentDialogue();
}

void ASweetDreamsDialogueManager::OnRep_CurrentDialogue()
{
	if (!Dialogues.IsValidIndex(CurrentDialogueID))
	{
		UE_LOG(LogTemp, Warning, TEXT("current dialogue ID not valid index, ENDING."));
		EndDialogue();
		return;
	}
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
	MulticastEndDialogue();
}

void ASweetDreamsDialogueManager::MulticastEndDialogue_Implementation()
{
	bIsDialogueActive = false;
	CurrentDialogue = FSweetDreamsDialogue();
	DialogueLog.Empty();
	CurrentDialogueID = -1;
	if (!bCanRepeatDialogue)
	{
		bIsDialogueEnabled = false;
	}
	if (bPossessThis)
	{
		SetViewTarget(nullptr, EndViewBlend);
	}
	if (bHideCharacter)
	{
		ToggleCharacterVisibility();
	}
	HideWidget();
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

void ASweetDreamsDialogueManager::CreateShowWidget()
{
	if (!IsValid(DialogueWidgetClass)) return;
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (!PC->IsLocalController()) return;
		if (ASweetDreamsHUD* HUD = Cast<ASweetDreamsHUD>(PC->GetHUD()))
		{
			DialogueWidget = Cast<UDialogueWidget>(HUD->CreateAndStoreWidget(DialogueWidgetClass));
			HUD->ShowWidget(DialogueWidget);
		}
		else
		{
			DialogueWidget = CreateWidget<UDialogueWidget>(PC, DialogueWidgetClass);
			if (IsValid(DialogueWidget))
			{
				DialogueWidget->AddToViewport();
			}
		}
	}
	if (IsValid(DialogueWidget))
	{
		DialogueWidget->SetDialogueManager(this);
		DialogueWidget->OnDialogueStarted();
	}
}

void ASweetDreamsDialogueManager::ToggleCharacterVisibility(bool bVisible)
{
	if (ACharacter* Character = Cast<ACharacter>(GetWorld()->GetFirstLocalPlayerFromController()))
	{
		Character->SetActorHiddenInGame(!bVisible);
	}
}

void ASweetDreamsDialogueManager::SetViewTarget(AActor* ViewTarget, const float& ViewTime)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (!IsValid(ViewTarget))
		{
			ViewTarget = PC->GetCharacter();
		}
		if (!IsValid(ViewTarget)) return;
		PC->SetViewTargetWithBlend(ViewTarget, ViewTime);
	}
}

void ASweetDreamsDialogueManager::HideWidget()
{
	if (!IsValid(DialogueWidget)) return;
	DialogueWidget->OnDialogueEnded();
	DialogueWidget->SetDialogueManager(nullptr);
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (!PC->IsLocalController()) return;
		if (ASweetDreamsHUD* HUD = Cast<ASweetDreamsHUD>(PC->GetHUD()))
		{
			DialogueWidget->HideSelf();
		}
		else
		{
			DialogueWidget->RemoveFromViewport();
		}
	}
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
	DialogueData = Data;
	OnRep_DialogueData();
}

void ASweetDreamsDialogueManager::OnRep_DialogueData()
{
	if (!IsValid(DialogueData)) return;
	UE_LOG(LogTemp, Warning, TEXT("Replicating dialogue data."));
	UpdateDialogueName(DialogueData->Name);
	Dialogues = DialogueData->Dialogues;
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




