// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/SweetDreamsWidget.h"

void USweetDreamsWidget::ShowWidget()
{
	OnShowRequested.ExecuteIfBound(this);
}

void USweetDreamsWidget::HideWidget()
{
	OnHideRequested.ExecuteIfBound(this);
}

void USweetDreamsWidget::ShowWidget_Internal()
{
	if (GetVisibility() == ESlateVisibility::SelfHitTestInvisible) return;

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	OnShow();
	OnWidgetShow.Broadcast(this);
}

void USweetDreamsWidget::HideWidget_Internal()
{
	if (GetVisibility() == ESlateVisibility::Collapsed) return;

	SetVisibility(ESlateVisibility::Collapsed);

	OnHide();
	OnWidgetHide.Broadcast(this);
}


