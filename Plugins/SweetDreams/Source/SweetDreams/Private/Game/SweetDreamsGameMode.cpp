// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SweetDreamsGameMode.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Player/SweetDreamsCharacter.h"
#include "Core/SweetDreamsSettings.h"
#include "Player/SweetDreamsHUD.h"
#include "Game/LoadingWidget.h"
#include "Player/SweetDreamsPlayerController.h"

ASweetDreamsGameMode::ASweetDreamsGameMode()
{
	DefaultPawnClass = ASweetDreamsCharacter::StaticClass();
	PlayerControllerClass = ASweetDreamsPlayerController::StaticClass();
	HUDClass = ASweetDreamsHUD::StaticClass();
}

void ASweetDreamsGameMode::BeginPlay()
{
	Core = GetGameInstance()->GetSubsystem<USweetDreamsCore>();
	if (IsValid(Core) && Core->CoreSettings)
	{
		if (Core->CoreSettings->bEnableAutoLoadData && Core->CoreSettings->bEnableAutoLoadSave)
		{
			Core->ManageSaveData(false);
			Core->ManageSaveData(false, false);
		}
	}
	LoadingWidget = CreateLoadingWidget(LoadingWidgetClass, bShowLoadingScreenOnBeginPlay);
	Super::BeginPlay();
}

ULoadingWidget* ASweetDreamsGameMode::CreateLoadingWidget(TSubclassOf<ULoadingWidget> Class, bool bAddToViewport)
{
	if (!IsValid(Class)) return nullptr;
	ULoadingWidget* NewWidget = CreateWidget<ULoadingWidget>(GetWorld(), Class);
	if (IsValid(NewWidget))
	{
		if (bAddToViewport) NewWidget->AddToViewport(99);
		if (LoadingDelay < 0.f) LoadingDelay = 0.1f;
		FTimerHandle WidgetRemoveTimer;
		GetWorld()->GetTimerManager().SetTimer(WidgetRemoveTimer, [this, NewWidget]()
			{
				NewWidget->OnLoadingDelayFinished(LoadingDelay);
				if (bHideLoadingScreenOnFinish) NewWidget->SetVisibility(ESlateVisibility::Collapsed);
			}, LoadingDelay, false);
	}
	return NewWidget;
}

void ASweetDreamsGameMode::LevelLoadStarted(TSoftObjectPtr<UWorld> LoadingLevel)
{
	if (IsValid(LoadingWidget))
	{
		LoadingWidget->OnLoadingStart();
	}
}

void ASweetDreamsGameMode::LevelLoadFinished(TSoftObjectPtr<UWorld> LoadingLevel)
{
	if (IsValid(LoadingWidget))
	{
		LoadingWidget->OnLoadingFinish();
		if (bHideLoadingScreenOnFinish) LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}


