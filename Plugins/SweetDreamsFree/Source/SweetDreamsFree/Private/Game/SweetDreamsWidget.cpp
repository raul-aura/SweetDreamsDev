// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/SweetDreamsWidget.h"
//#include "Player/SweetDreamsHUD.h"

USweetDreamsWidget::USweetDreamsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputMode = EInputMode::UI;
}

void USweetDreamsWidget::HideSelf()
{
	//if (ASweetDreamsHUD* SweetDreamsHUD = Cast<ASweetDreamsHUD>(GetOwningPlayer()->GetHUD()))
	//{
	//	SweetDreamsHUD->HideWidget(this);
	//}
}

void USweetDreamsWidget::HUDShow()
{
	if (!bWidgetShowed)
	{
		OnFirstShow();
		bWidgetShowed = true;
		return;
	}
	OnShow();
}
