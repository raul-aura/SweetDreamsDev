// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsItem.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

void USweetDreamsItem::SetItemOwner(AActor* NewOwner)
{
    if (!IsValid(NewOwner)) return;
    ItemOwner = NewOwner;
}

void USweetDreamsItem::IncreaseAmount()
{
    Amount = FMath::Min(++Amount, MaxAmountOrCopies);
}

void USweetDreamsItem::DecreaseAmount()
{
    Amount = FMath::Max(--Amount, 0);
}
