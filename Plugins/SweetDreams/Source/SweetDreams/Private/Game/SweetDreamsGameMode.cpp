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
	SweetDreamsCore = USweetDreamsBPLibrary::GetSweetDreamsCore(this);
	if (IsValid(SweetDreamsCore))
	{
		SweetDreamsCore->LoadCoreSaves();
	}
	LoadingWidget = CreateLoadingWidget(LoadingWidgetClass, bShowOnBeginPlay);
	Super::BeginPlay();
}

ULoadingWidget* ASweetDreamsGameMode::CreateLoadingWidget(TSubclassOf<ULoadingWidget> Class, bool bAddToViewport)
{
	if (!IsValid(Class)) return nullptr;
	ULoadingWidget* NewWidget = CreateWidget<ULoadingWidget>(GetWorld(), Class);
	if (IsValid(NewWidget))
	{
		if (bAddToViewport) NewWidget->AddToViewport(99);
		if (WidgetGracePeriod < 0.f) WidgetGracePeriod = 0.01f;
		FTimerHandle WidgetRemoveTimer;
		GetWorld()->GetTimerManager().SetTimer(WidgetRemoveTimer, [this, NewWidget]()
			{
				NewWidget->OnGracePeriodEnded(WidgetGracePeriod);
				if (bHideAfterGracePeriod) NewWidget->SetVisibility(ESlateVisibility::Collapsed);
			}, WidgetGracePeriod, false);
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
	}
}


