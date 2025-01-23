// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsGameMode.h"
#include "SweetDreamsBPLibrary.h"
#include "SweetDreamsCharacter.h"
#include "SweetDreamsSettings.h"
#include "SweetDreamsPlayerController.h"

ASweetDreamsGameMode::ASweetDreamsGameMode()
{
	DefaultPawnClass = ASweetDreamsCharacter::StaticClass();
	PlayerControllerClass = ASweetDreamsPlayerController::StaticClass();
}

void ASweetDreamsGameMode::BeginPlay()
{
	Core = GetGameInstance()->GetSubsystem<USweetDreamsCore>();
	if (Core && Core->CoreSettings)
	{
		if (Core->CoreSettings->bEnableAutoLoadData && Core->CoreSettings->bEnableAutoLoadSave)
		{
			Core->ManageSaveData(false);
			Core->ManageSaveData(false, false);
		}
	}
	LoadingWidget = CreateLoadingWidget(LoadingWidgetClass);
	Super::BeginPlay();
}

ULoadingWidget* ASweetDreamsGameMode::CreateLoadingWidget(TSubclassOf<ULoadingWidget> Class)
{
	if (!Class) return nullptr;
	ULoadingWidget* NewWidget = CreateWidget<ULoadingWidget>(GetWorld(), Class);
	if (NewWidget)
	{
		NewWidget->AddToViewport(99);
		FTimerHandle WidgetRemoveTimer;
		GetWorld()->GetTimerManager().SetTimer(WidgetRemoveTimer, [NewWidget]()
			{
				NewWidget->SetVisibility(ESlateVisibility::Collapsed);
			}, LoadingDelay, false);
	}
	return NewWidget;
}


