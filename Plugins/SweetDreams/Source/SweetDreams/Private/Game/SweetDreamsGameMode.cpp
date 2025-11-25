// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/SweetDreamsGameMode.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Player/SweetDreamsCharacter.h"
#include "Core/SweetDreamsSettings.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "Player/SweetDreamsHUD.h"
#include "Player/SweetDreamsPlayerController.h"

ASweetDreamsGameMode::ASweetDreamsGameMode()
{
	DefaultPawnClass = ASweetDreamsCharacter::StaticClass();
	PlayerControllerClass = ASweetDreamsPlayerController::StaticClass();
	HUDClass = ASweetDreamsHUD::StaticClass();
}

void ASweetDreamsGameMode::StartPlay()
{
	SweetDreamsCore = USweetDreamsBPLibrary::GetSweetDreamsCore(this);
	if (IsValid(SweetDreamsCore) && SweetDreamsCore->CoreSettings->bEnableAutoLoadSave)
	{
		SweetDreamsCore->LoadCoreSaves();
	}
	Super::StartPlay();
}

void ASweetDreamsGameMode::BeginPlay()
{
	if (IsValid(SweetDreamsCore) && SweetDreamsCore->bIsLoadingLevel) {
		BeginPlayNewLevel(SweetDreamsCore->CurrentLoadingLevel.Get());
	}

	Super::BeginPlay();
}

void ASweetDreamsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ConnectedPlayers.Add(NewPlayer);
}

void ASweetDreamsGameMode::Logout(AController* Exiting)
{
	APlayerController* ExitingPlayer = Cast<APlayerController>(Exiting);
	if (ConnectedPlayers.Num() > 0 && IsValid(ExitingPlayer) && ConnectedPlayers.Contains(ExitingPlayer))
	{
		ConnectedPlayers.Remove(ExitingPlayer);
	}
	Super::Logout(Exiting);
}

void ASweetDreamsGameMode::StartLoadingLevel(TObjectPtr<UWorld> LoadingLevel)
{
	OnLevelLoadStarted(LoadingLevel.Get());

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ASweetDreamsHUD* HUD = Cast<ASweetDreamsHUD>(PC->GetHUD()))
		{
			HUD->CreateLoadingWidget(LoadingWidgetClass);
			HUD->ShowLoadingWidget();
		}
	}
}

void ASweetDreamsGameMode::FinishLoadingLevel(TObjectPtr<UWorld> LoadingLevel)
{
	OnLevelLoadFinished(LoadingLevel.Get());

}

void ASweetDreamsGameMode::BeginPlayNewLevel(TObjectPtr<UWorld> LoadingLevel)
{
	if (IsValid(SweetDreamsCore)) {

		// create then destroy loading widget

		SweetDreamsCore->bIsLoadingLevel = false;
		SweetDreamsCore->CurrentLoadingLevel = nullptr;
	}
}

bool ASweetDreamsGameMode::KickPlayer(APlayerController* KickedPlayer, const FText& KickReason)
{
	if (IsValid(GameSession) && IsValid(KickedPlayer))
	{
		return GameSession->KickPlayer(KickedPlayer, KickReason);
	}
	return false;
}

TArray<APlayerController*> ASweetDreamsGameMode::GetConnectedPlayers(bool bExcludeLocalPlayer) const
{
	if (bExcludeLocalPlayer)
	{
		TArray<APlayerController*> PlayersNoLocal = ConnectedPlayers;
		APlayerController* LocalPlayerC = nullptr;
		for (auto& Player : PlayersNoLocal)
		{
			if (IsValid(Player) && Player->IsLocalController())
			{
				LocalPlayerC = Player;
				break;
			}
		}
		if (IsValid(LocalPlayerC)) PlayersNoLocal.Remove(LocalPlayerC);
		return PlayersNoLocal;
	}
	return ConnectedPlayers;
}


