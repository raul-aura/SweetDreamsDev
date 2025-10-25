// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/SweetDreamsWidget.h"

void USweetDreamsWidget::ShowWidget()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OnShow();
}

void USweetDreamsWidget::HideWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
	OnHide();
}


