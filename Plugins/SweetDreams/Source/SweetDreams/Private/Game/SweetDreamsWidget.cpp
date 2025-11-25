// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/SweetDreamsWidget.h"

void USweetDreamsWidget::ShowWidget()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	OnShow();
	OnWidgetShow.Broadcast(this);
}

void USweetDreamsWidget::HideWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);

	OnHide();
	OnWidgetHide.Broadcast(this);
}


