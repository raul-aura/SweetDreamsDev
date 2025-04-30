// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UInventoryComponent* UInventoryComponent::GetInventoryFromActor(const AActor* Actor)
{
	UWorld* World = GEngine->GetWorldFromContextObject(Actor, EGetWorldErrorMode::ReturnNull);
	if (!IsValid(Actor) && !IsValid(World))
	{
		return nullptr;
	}
	UInventoryComponent* Component = Actor->FindComponentByClass<UInventoryComponent>();
	return Component;
}

void UInventoryComponent::AddItem(const TSoftObjectPtr<USweetDreamsItem>& Item)
{
    if (Item.IsValid())
    {
        Items.Add(FInventoryItem(Item));
    }
}

void UInventoryComponent::UseItem(int32 Index)
{
    if (Items.IsValidIndex(Index))
    {
        FInventoryItem& Item = Items[Index];
        if (USweetDreamsItem* ItemData = Item.GetItemData())
        {
            
        }
    }
}

void UInventoryComponent::InspectItem(int32 Index)
{
    if (Items.IsValidIndex(Index))
    {
        FInventoryItem& Item = Items[Index];
        if (USweetDreamsItem* ItemData = Item.GetItemData())
        {

        }
    }
}

void UInventoryComponent::UseItemByName(FName ItemName)
{
    const int32 Index = FindItemIndex(ItemName);
    if (Index != INDEX_NONE)
    {
        UseItem(Index);
    }
}

void UInventoryComponent::InspectItemByName(FName ItemName)
{
    const int32 Index = FindItemIndex(ItemName);
    if (Index != INDEX_NONE)
    {
        InspectItem(Index);
    }
}

int32 UInventoryComponent::GetItemCountByName(FName ItemName) const
{
    return INDEX_NONE;
}

bool UInventoryComponent::HasItem(FName ItemName) const
{
    return FindItemIndex(ItemName) != INDEX_NONE;
}

int32 UInventoryComponent::FindItemIndex(FName ItemName) const
{
    return INDEX_NONE;
}

