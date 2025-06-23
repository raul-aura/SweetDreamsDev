// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/SweetDreamsItem.h"
#include "Inventory/ItemEvent.h"

void USweetDreamsItem::AssignItemToEvents()
{
	for (auto* Event : ItemEvents)
	{
		Event->CurrentItemData = this;
	}
}
