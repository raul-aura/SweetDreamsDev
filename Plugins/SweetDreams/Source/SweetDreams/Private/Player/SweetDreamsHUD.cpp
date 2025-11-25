// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/SweetDreamsHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LoadingWidget.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Game/SweetDreamsWidget.h"

void ASweetDreamsHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CreateStartingWidgets();
}

void ASweetDreamsHUD::CreateStartingWidgets()
{
	AllWidgets.Empty();
	if (StartingWidgets.Num() == 0) return;
	for (TSubclassOf<USweetDreamsWidget> Widget : StartingWidgets)
	{
		USweetDreamsWidget* NewWidget = CreateWidget<USweetDreamsWidget>(GetOwningPlayerController(), Widget);
		if (IsValid(NewWidget))
		{
			NewWidget->SetVisibility(ESlateVisibility::Collapsed);
			NewWidget->AddToViewport(NewWidget->GetInitialZOrder());

			NewWidget->OnWidgetShow.AddUniqueDynamic(this, &ASweetDreamsHUD::ShowWidget);
			NewWidget->OnWidgetHide.AddUniqueDynamic(this, &ASweetDreamsHUD::HideWidget);

			AllWidgets.Add(NewWidget);
		}
	}
}

USweetDreamsWidget* ASweetDreamsHUD::CreateAndStoreWidget(TSubclassOf<USweetDreamsWidget> WidgetClass)
{
	if (!IsValid(WidgetClass)) return nullptr;
	USweetDreamsWidget* NewWidget = CreateWidget<USweetDreamsWidget>(GetOwningPlayerController(), WidgetClass);
	if (IsValid(NewWidget))
	{
		NewWidget->SetVisibility(ESlateVisibility::Collapsed);
		NewWidget->AddToViewport(NewWidget->GetInitialZOrder());

		NewWidget->OnWidgetShow.AddUniqueDynamic(this, &ASweetDreamsHUD::ShowWidget);
		NewWidget->OnWidgetHide.AddUniqueDynamic(this, &ASweetDreamsHUD::HideWidget);

		AllWidgets.Add(NewWidget);
	}
	return NewWidget;
}

void ASweetDreamsHUD::ShowWidget(USweetDreamsWidget* Widget)
{
	if (!IsValid(Widget)) return;
	UpdatePlayerInputMode();
	Widget->ShowWidget();
}

void ASweetDreamsHUD::HideWidget(USweetDreamsWidget* Widget)
{
	if (!IsValid(Widget)) return;
	UpdatePlayerInputMode();
	Widget->HideWidget();
}

void ASweetDreamsHUD::OverridePlayerInputMode(USweetDreamsWidget* WidgetToFocus)
{
	EInputMode NewInputMode = EInputMode::GAME;
	if (IsValid(WidgetToFocus))
	{
		NewInputMode = WidgetToFocus->InputMode;
	}
	if (APlayerController* Player = GetOwningPlayerController())
	{
		switch (NewInputMode)
		{
		case EInputMode::GAMEANDUI:
		{
			FInputModeGameAndUI Mode;
			if (WidgetToFocus) Mode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
			Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			Player->SetInputMode(Mode);
			Player->SetShowMouseCursor(true);
			break;
		}
		case EInputMode::UI:
		{
			FInputModeUIOnly Mode;
			if (WidgetToFocus) Mode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
			Player->SetInputMode(Mode);
			Player->SetShowMouseCursor(true);
			break;
		}
		case EInputMode::GAME:
		default:
		{
			Player->SetInputMode(FInputModeGameOnly());
			Player->SetShowMouseCursor(false);
			break;
		}
		}
	}
}

void ASweetDreamsHUD::UpdatePlayerInputMode()
{
	USweetDreamsWidget* TopWidget = GetHighestPriorityWidget();
	OverridePlayerInputMode(TopWidget);
}

ULoadingWidget* ASweetDreamsHUD::CreateLoadingWidget(TSubclassOf<ULoadingWidget> WidgetClass)
{
	LoadingWidget = Cast<ULoadingWidget>(CreateAndStoreWidget(WidgetClass));
	return LoadingWidget;
}

void ASweetDreamsHUD::ShowLoadingWidget()
{
	ShowWidget(LoadingWidget);
}

void ASweetDreamsHUD::HideLoadingWidget()
{
	HideWidget(LoadingWidget);
}

ULoadingWidget* ASweetDreamsHUD::GetLoadingWidget() const
{
	return LoadingWidget;
}

bool ASweetDreamsHUD::IsAnyWidgetVisible() const
{
	return AllWidgets.ContainsByPredicate([](const USweetDreamsWidget* Widget)
	{
		return !Widget->bIgnoreThisForVisibility && Widget->IsVisible();
	});
}

USweetDreamsWidget* ASweetDreamsHUD::GetHighestPriorityWidget() const
{
	USweetDreamsWidget* TopWidget = nullptr;
	int32 HighestZOrder = TNumericLimits<int32>::Min();
	for (USweetDreamsWidget* Widget : AllWidgets)
	{
		if (!IsValid(Widget) || Widget->bIgnoreThisForVisibility || !Widget->IsVisible())
		{
			continue;
		}
		if (Widget->GetInitialZOrder() > HighestZOrder)
		{
			HighestZOrder = Widget->GetInitialZOrder();
			TopWidget = Widget;
		}
	}
	return TopWidget;
}

USweetDreamsWidget* ASweetDreamsHUD::FindWidgetByClass(TSubclassOf<USweetDreamsWidget> WidgetClass) const
{
	if (!WidgetClass || AllWidgets.Num() == 0) return nullptr;
	for (USweetDreamsWidget* Widget : AllWidgets)
	{
		if (IsValid(Widget) && Widget->IsA(WidgetClass)) return Widget;
	}
	return nullptr;
}

USweetDreamsWidget* ASweetDreamsHUD::FindWidgetByName(FName WidgetName) const
{
	if (WidgetName.IsNone() || AllWidgets.Num() == 0) return nullptr;
	for (USweetDreamsWidget* Widget : AllWidgets)
	{
		if (IsValid(Widget) && Widget->WidgetName.IsEqual(WidgetName)) return Widget;
	}
	return nullptr;
}


