// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SweetDreamsPlayerController.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Core/SweetDreamsSettings.h"
#include "Player/SweetDreamsCharacter.h"

ASweetDreamsCharacter* ASweetDreamsPlayerController::GetDreamCharacter() const
{
	return Cast<ASweetDreamsCharacter>(GetCharacter());
}

void ASweetDreamsPlayerController::MoveForward(float Value)
{
	if (ASweetDreamsCharacter* DreamChar = GetDreamCharacter())
	{
		DreamChar->MoveForward(Value);
	}
}

void ASweetDreamsPlayerController::MoveRight(float Value)
{
	if (ASweetDreamsCharacter* DreamChar = GetDreamCharacter())
	{
		DreamChar->MoveRight(Value);
	}
}

void ASweetDreamsPlayerController::Run()
{
	if (ASweetDreamsCharacter* DreamChar = GetDreamCharacter())
	{
		DreamChar->Run();
	}
}

void ASweetDreamsPlayerController::StopRunning()
{
	if (ASweetDreamsCharacter* DreamChar = GetDreamCharacter())
	{
		DreamChar->StopRunning();
	}
}

void ASweetDreamsPlayerController::CameraVertical(float Value, float Sensitivity)
{
	if (ASweetDreamsCharacter* DreamChar = GetDreamCharacter())
	{
		DreamChar->CameraVertical(Value, Sensitivity);
	}
}

void ASweetDreamsPlayerController::CameraHorizontal(float Value, float Sensitivity)
{
	if (ASweetDreamsCharacter* DreamChar = GetDreamCharacter())
	{
		DreamChar->CameraHorizontal(Value, Sensitivity);
	}
}

void ASweetDreamsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController())
	{
		USweetDreamsCore* SweetDreamsCore = USweetDreamsBPLibrary::GetSweetDreamsCore(this);
		if (IsValid(SweetDreamsCore) && SweetDreamsCore->CoreSettings->bEnableAutoLoadSave)
		{
			SweetDreamsCore->LoadCoreSaves();
		}
	}
}

void ASweetDreamsPlayerController::ToggleInputTimer(float Duration)
{
	FTimerHandle InputTimer;
	DisableInput(this);
	GetWorldTimerManager().SetTimer(InputTimer, [this]() {
		EnableInput(this);
	}, Duration, false);
}