// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UInventoryComponent* UInventoryComponent::GetInventoryFromActor(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}
	UInventoryComponent* Component = Actor->FindComponentByClass<UInventoryComponent>();
	return Component;
}

USweetDreamsItem* UInventoryComponent::GetItemData(const FInventoryItem& Item)
{
    return Item.GetItemData();
}

void UInventoryComponent::AddItem(const TSoftObjectPtr<USweetDreamsItem>& ItemData, int32 Count, bool bAddAsUnique)
{
    if (ItemData.IsValid())
    {
        if (!bAddAsUnique)
        {
            for (FInventoryItem& ItemF : Items)
            {
                if (ItemF.ItemDataPtr == ItemData)
                {
                    ItemF.Count += Count;
                    return;
                }
            }
        }
        Items.Add(FInventoryItem(ItemData));
    }
}

void UInventoryComponent::UseItem(const FInventoryItem& Item)
{
    if (Item.IsValid())
    {
        Item.GetItemData();
    }
}

void UInventoryComponent::InspectItem(const FInventoryItem& Item)
{

}

bool UInventoryComponent::HasItem(const TSoftObjectPtr<USweetDreamsItem>& ItemData, FInventoryItem& FoundItem) const
{
    if (ItemData.IsValid())
    {
        for (const FInventoryItem& ItemF : Items)
        {
            if (ItemF.ItemDataPtr == ItemData)
            {
                FoundItem = ItemF;
                return true;
            }
        }
    }
    return false;
}

FInventoryItem UInventoryComponent::GetItemByIndex(int32 Index, bool& bFound) const
{
    bFound = false;
    if (Items.Num() > 0 && Items.IsValidIndex(Index))
    {
        bFound = true;
        return Items[Index];
    }
    return FInventoryItem();
}

