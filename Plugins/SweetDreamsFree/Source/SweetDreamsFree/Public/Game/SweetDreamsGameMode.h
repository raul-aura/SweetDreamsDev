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
	
	ASweetDreamsGameMode();

public:
	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
	//UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core")
	//void CreateLoadingWidget(TSubclassOf<ULoadingWidget> Class, ASweetDreamsPlayerController* Player);
	//void ShowLoadingWidget(ASweetDreamsPlayerController* Player);
	//void LevelLoadStarted(TSoftObjectPtr<UWorld> LoadingLevel);
	//void LevelLoadFinished(TSoftObjectPtr<UWorld> LoadingLevel);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core")
	bool KickPlayer(APlayerController* KickedPlayer, const FText& KickReason);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core")
	TArray<APlayerController*> GetConnectedPlayers(bool bExcludeLocalPlayer = false) const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	USweetDreamsCore* SweetDreamsCore = nullptr;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen")
	//TSubclassOf<ULoadingWidget> LoadingWidgetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen")
	bool bShowOnBeginPlay = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen")
	bool bShowOnStartLoading = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen", meta = (ClampMin = "0"))
	float WidgetGracePeriod = 3.f; // grace period for loading widget, called on begin play 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen")
	bool bHideAfterGracePeriod = true; // when grace period ends, should auto hide the widget?

	TArray<APlayerController*> ConnectedPlayers;
};
