// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsHUD.h"
#include "Kismet/GameplayStatics.h"
#include "SweetDreamsBPLibrary.h"
#include "SweetDreamsWidget.h"

TArray<USweetDreamsWidget*> ASweetDreamsHUD::AllWidgets;

void ASweetDreamsHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CreateWidgets();
}

void ASweetDreamsHUD::CreateWidgets()
{
	AllWidgets.Empty();
	if (DefaultWidgets.Num() == 0) return;
	for (TSubclassOf<USweetDreamsWidget> Widget : DefaultWidgets)
	{
		USweetDreamsWidget* NewWidget = CreateWidget<USweetDreamsWidget>(GetOwningPlayerController(), Widget);
		if (IsValid(NewWidget))
		{
			NewWidget->SetVisibility(ESlateVisibility::Collapsed);
			NewWidget->AddToViewport();
			AllWidgets.Add(NewWidget);
		}
	}
}

void ASweetDreamsHUD::CreateAndStoreWidget(TSubclassOf<USweetDreamsWidget> WidgetClass)
{
	if (!IsValid(WidgetClass)) return;
	USweetDreamsWidget* NewWidget = CreateWidget<USweetDreamsWidget>(GetOwningPlayerController(), WidgetClass);
	if (IsValid(NewWidget))
	{
		NewWidget->SetVisibility(ESlateVisibility::Collapsed);
		NewWidget->AddToViewport(NewWidget->GetInitialZOrder());
		AllWidgets.Add(NewWidget);
	}
}

void ASweetDreamsHUD::ShowWidget(USweetDreamsWidget* Widget)
{
	if (!IsValid(Widget)) return;
	Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UpdatePlayerInputMode(Widget);
	Widget->HUDShow();
}

void ASweetDreamsHUD::HideWidget(USweetDreamsWidget* Widget)
{
	if (!IsValid(Widget)) return;
	Widget->SetVisibility(ESlateVisibility::Collapsed);
	if (!IsAnyWidgetVisible())
	{
		UpdatePlayerInputMode(Widget);
	}
	Widget->OnHide();
}

USweetDreamsWidget* ASweetDreamsHUD::FindWidgetByClass(TSubclassOf<USweetDreamsWidget> WidgetClass)
{
	if (!WidgetClass || AllWidgets.Num() == 0) return nullptr;
	for (USweetDreamsWidget* Widget : AllWidgets)
	{
		if (IsValid(Widget) && Widget->IsA(WidgetClass)) return Widget;
	}
	return nullptr;
}

USweetDreamsWidget* ASweetDreamsHUD::FindWidgetByName(FName WidgetName)
{
	if (WidgetName.IsNone() || AllWidgets.Num() == 0) return nullptr;
	for (USweetDreamsWidget* Widget : AllWidgets)
	{
		if (IsValid(Widget) && Widget->WidgetName.IsEqual(WidgetName)) return Widget;
	}
	return nullptr;
}

void ASweetDreamsHUD::UpdatePlayerInputMode(USweetDreamsWidget* WidgetToFocus)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WidgetToFocus, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WidgetToFocus), TEXT("World Context was not valid.")))
	{
		return;
	}
	if (APlayerController* Player = World->GetFirstPlayerController())
	{
		if (IsAnyWidgetVisible())
		{
			EInputMode NewInputMode = WidgetToFocus->InputMode;
			switch (NewInputMode)
			{
			case EInputMode::GAMEANDUI:
			{
				FInputModeGameAndUI ModeGameUI;
				ModeGameUI.SetWidgetToFocus(WidgetToFocus->TakeWidget());
				ModeGameUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				Player->SetInputMode(ModeGameUI);
				Player->SetShowMouseCursor(true);
				return;
			}
			case EInputMode::GAME:
			{
				FInputModeGameOnly ModeGame;
				Player->SetInputMode(ModeGame);
				Player->SetShowMouseCursor(false);
				return;
			}
			case EInputMode::UI:
			{
				FInputModeUIOnly ModeUI;
				ModeUI.SetWidgetToFocus(WidgetToFocus->TakeWidget());
				Player->SetInputMode(ModeUI);
				Player->SetShowMouseCursor(true);
				return;
			}
			default:
				break;
			}
		}
		else
		{
			Player->SetInputMode(FInputModeGameOnly());
			Player->SetShowMouseCursor(false);
		}
	}
}

bool ASweetDreamsHUD::IsAnyWidgetVisible()
{
	if (AllWidgets.Num() == 0) return false;
	for (USweetDreamsWidget* Widget : AllWidgets)
	{
		if (Widget->IsVisible() && !Widget->bIgnoreThisForVisibility)
		{
			return true;
		}
	}
	return false;
}


