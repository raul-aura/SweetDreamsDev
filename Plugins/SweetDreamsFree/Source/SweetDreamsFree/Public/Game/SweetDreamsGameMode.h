// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SweetDreamsGameMode.generated.h"

class USweetDreamsCore;
class ASweetDreamsPlayerController;
struct FDreamUserSettings;

UCLASS()
class SWEETDREAMSFREE_API ASweetDreamsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASweetDreamsGameMode();

	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Core")
	void OnLevelLoadStarted(UWorld* LoadingLevel);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Core")
	void OnLevelLoadFinished(UWorld* LoadingLevel);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core")
	bool KickPlayer(APlayerController* KickedPlayer, const FText& KickReason);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core")
	TArray<APlayerController*> GetConnectedPlayers(bool bExcludeLocalPlayer = false) const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	USweetDreamsCore* SweetDreamsCore = nullptr;

	TArray<APlayerController*> ConnectedPlayers;
};
