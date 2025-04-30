// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsWidget.h"

void USweetDreamsWidget::HideSelf()
{
	ASweetDreamsHUD::HideWidget(this);
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
