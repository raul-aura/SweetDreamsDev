// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/SweetDreamsGameMode.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Player/SweetDreamsCharacter.h"
#include "Core/SweetDreamsSettings.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "Player/SweetDreamsPlayerController.h"

ASweetDreamsGameMode::ASweetDreamsGameMode()
{
	DefaultPawnClass = ASweetDreamsCharacter::StaticClass();
	PlayerControllerClass = ASweetDreamsPlayerController::StaticClass();
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


