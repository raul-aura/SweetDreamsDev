// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/SweetDreamsGameSession.h"

ASweetDreamsGameSession::ASweetDreamsGameSession()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool ASweetDreamsGameSession::KickPlayer(APlayerController* KickedPlayer, const FText& KickReason)
{
	return Super::KickPlayer(KickedPlayer, KickReason);
}

TArray<APlayerController*> ASweetDreamsGameSession::GetPlayersFromSession(bool bExcludeLocalPlayer) const
{
	TArray<APlayerController*> Controllers;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (IsValid(PC) && (!bExcludeLocalPlayer || !PC->IsLocalController()))
		{
			Controllers.Add(PC);
		}
	}
	return Controllers;
}




