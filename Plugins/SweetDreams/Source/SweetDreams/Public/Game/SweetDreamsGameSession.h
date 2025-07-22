// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "SweetDreamsGameSession.generated.h"

UCLASS()
class SWEETDREAMS_API ASweetDreamsGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:	
	ASweetDreamsGameSession();

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Game Session")
	bool KickPlayer(APlayerController* KickedPlayer, const FText& KickReason) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Game Session")
	TArray<APlayerController*> GetPlayersFromSession(bool bExcludeLocalPlayer = false) const;

protected:

};
