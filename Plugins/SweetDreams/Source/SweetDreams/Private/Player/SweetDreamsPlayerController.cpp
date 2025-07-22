// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SweetDreamsPlayerController.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Core/SweetDreamsSettings.h"
#include "Game/LoadingWidget.h"

ASweetDreamsCharacter* ASweetDreamsPlayerController::GetDreamCharacter() const
{
	return Cast<ASweetDreamsCharacter>(GetCharacter());
}

ASweetDreamsHUD* ASweetDreamsPlayerController::GetDreamHUD() const
{
	return GetHUD<ASweetDreamsHUD>();
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

void ASweetDreamsPlayerController::Client_CreateLoadingWidget_Implementation(TSubclassOf<ULoadingWidget> WidgetClass)
{
	if (IsValid(GetDreamHUD()))
	{
		GetDreamHUD()->CreateLoadingWidget(WidgetClass);
	}
}

void ASweetDreamsPlayerController::Client_ShowLoadingWidget_Implementation()
{
	if (IsValid(GetDreamHUD()))
	{
		GetDreamHUD()->ShowLoadingWidget();
	}
}

void ASweetDreamsPlayerController::Client_LoadingGracePeriodEnd_Implementation(float GracePeriod)
{
	if (IsValid(GetDreamHUD()) && GetDreamHUD()->GetLoadingWidget())
	{
		GetDreamHUD()->GetLoadingWidget()->OnGracePeriodEnded(GracePeriod);
	}
}

void ASweetDreamsPlayerController::Client_HideLoadingWidget_Implementation()
{
	if (IsValid(GetDreamHUD()))
	{
		GetDreamHUD()->HideLoadingWidget();
	}
}

void ASweetDreamsPlayerController::Client_LoadingStart_Implementation()
{
	if (IsValid(GetDreamHUD()) && GetDreamHUD()->GetLoadingWidget())
	{
		GetDreamHUD()->GetLoadingWidget()->OnLoadingStart();
	}
	LoadingWidget->OnLoadingStart();
}

void ASweetDreamsPlayerController::Client_LoadingEnd_Implementation()
{
	if (IsValid(GetDreamHUD()) && GetDreamHUD()->GetLoadingWidget())
	{
		GetDreamHUD()->GetLoadingWidget()->OnLoadingFinish();
	}
}

void ASweetDreamsPlayerController::ClientWasKicked_Implementation(const FText& KickReason)
{
	OnPlayerKicked(KickReason);
}
