// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SweetDreamsGameMode.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Player/SweetDreamsCharacter.h"
#include "Core/SweetDreamsSettings.h"
#include "Player/SweetDreamsHUD.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Game/LoadingWidget.h"
#include "GameFramework/GameSession.h"
#include "Player/SweetDreamsPlayerController.h"

ASweetDreamsGameMode::ASweetDreamsGameMode()
{
	DefaultPawnClass = ASweetDreamsCharacter::StaticClass();
	PlayerControllerClass = ASweetDreamsPlayerController::StaticClass();
	//HUDClass = ASweetDreamsHUD::StaticClass();
	//LoadingWidgetClass = ULoadingWidget::StaticClass();
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
	//if (ASweetDreamsPlayerController* DreamController = Cast<ASweetDreamsPlayerController>(NewPlayer))
	//{
	//	CreateLoadingWidget(LoadingWidgetClass, DreamController);
	//	if (bShowOnBeginPlay) ShowLoadingWidget(DreamController);
	//}
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

//void ASweetDreamsGameMode::CreateLoadingWidget(TSubclassOf<ULoadingWidget> Class, ASweetDreamsPlayerController* Player)
//{
//	if (!IsValid(Class) || !IsValid(Player)) return;
//	WidgetGracePeriod = FMath::Max(WidgetGracePeriod, 0.01f);
//	Player->Client_CreateLoadingWidget(Class);
//	FTimerHandle WidgetRemoveTimer;
//	GetWorld()->GetTimerManager().SetTimer(WidgetRemoveTimer, [this, Player]()
//	{
//		Player->Client_LoadingGracePeriodEnd(WidgetGracePeriod);
//		if (bHideAfterGracePeriod)
//		{
//			Player->Client_HideLoadingWidget();
//		}
//	}, WidgetGracePeriod, false);
//}
//
//void ASweetDreamsGameMode::ShowLoadingWidget(ASweetDreamsPlayerController* Player)
//{
//	if (!IsValid(Player)) return;
//	Player->Client_ShowLoadingWidget();
//}
//
//void ASweetDreamsGameMode::LevelLoadStarted(TSoftObjectPtr<UWorld> LoadingLevel)
//{
//	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//	{
//		if (ASweetDreamsPlayerController* DreamController = Cast<ASweetDreamsPlayerController>(*It))
//		{
//			DreamController->Client_LoadingStart();
//			if (bShowOnStartLoading) DreamController->Client_ShowLoadingWidget();
//		}
//	}
//}
//
//void ASweetDreamsGameMode::LevelLoadFinished(TSoftObjectPtr<UWorld> LoadingLevel)
//{
//	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//	{
//		if (ASweetDreamsPlayerController* DreamController = Cast<ASweetDreamsPlayerController>(*It))
//		{
//			DreamController->Client_LoadingEnd();
//		}
//	}
//}

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


