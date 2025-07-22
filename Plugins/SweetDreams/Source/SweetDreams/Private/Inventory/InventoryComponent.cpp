// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/ItemEvent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UInventoryComponent, Items);
}

void UInventoryComponent::BeginPlay()
{
    PreviousItems = Items;
    Super::BeginPlay();
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

USweetDreamsItem* UInventoryComponent::GetItemData(const FInventoryItem& Item, bool& ValidData)
{
    ValidData = Item.IsItemValid();
    return Item.ItemData;
}

bool UInventoryComponent::IsItemValid(const FInventoryItem& Item)
{
    return Item.IsItemValid();
}

void UInventoryComponent::AddItem(const TSoftObjectPtr<USweetDreamsItem>& ItemData, int32 Count, bool bAddAsUnique)
{
    if (ItemData.IsValid())
    {
        USweetDreamsItem* ItemObject = ItemData.Get();
        if (!bAddAsUnique)
        {
            for (FInventoryItem& ItemF : Items)
            {
                if (ItemF.ItemData == ItemObject)
                {
                    ItemF.Count += Count;
                    for (auto Event : ItemF.ItemData->ItemEvents)
                    {
                        Event->OnAdded(GetOwner(), Count);
                    }
                    OnItemAdded.Broadcast(ItemF);
                    return;
                }
            }
        }
        FInventoryItem NewItem = FInventoryItem(ItemObject);
        NewItem.ItemData->AssignItemToEvents();
        for (auto Event : NewItem.ItemData->ItemEvents)
        {
            Event->OnAdded(GetOwner(), 1);
        }
        Items.Add(FInventoryItem(ItemObject));
        OnItemAdded.Broadcast(NewItem);
    }
}

void UInventoryComponent::AddItem_Internal(const TSoftObjectPtr<USweetDreamsItem>& ItemData, int32 Count, bool bAddAsUnique)
{

}

void UInventoryComponent::ServerAddItem_Implementation(const TSoftObjectPtr<USweetDreamsItem>& ItemData, int32 Count, bool bAddAsUnique)
{
    AddItem_Internal(ItemData, Count, bAddAsUnique);
}

void UInventoryComponent::UseItem(const FInventoryItem& Item)
{
    if (Item.IsItemValid())
    {
        for (auto* Event : Item.ItemData->ItemEvents)
        {
            Event->OnUsed(GetOwner());
        }
        OnItemUsed.Broadcast(Item);
    }
}

void UInventoryComponent::InspectItem(const FInventoryItem& Item)
{
    if (Item.IsItemValid())
    {
        for (auto* Event : Item.ItemData->ItemEvents)
        {
            Event->OnInspected(GetOwner());
        }
        OnItemInspected.Broadcast(Item);
    }
}

void UInventoryComponent::EquipItem(UPARAM(ref) FInventoryItem& Item)
{
    if (Item.IsItemValid())
    {
        Item.bIsEquipping = true;
        for (auto* Event : Item.ItemData->ItemEvents)
        {
            Event->OnEquiped(GetOwner());
        }
        OnItemEquipped.Broadcast(Item);
    }
}

void UInventoryComponent::UnequipItem(UPARAM(ref) FInventoryItem& Item)
{
    if (Item.IsItemValid())
    {
        Item.bIsEquipping = false;
        for (auto* Event : Item.ItemData->ItemEvents)
        {
            Event->OnUnequiped(GetOwner());
        }
        OnItemUnequiped.Broadcast(Item);
    }
}

void UInventoryComponent::RemoveItem(UPARAM(ref) FInventoryItem& Item)
{
    if (Item.IsItemValid())
    {
        Item.Count = FMath::Max(Item.Count--, 0);
        for (auto* Event : Item.ItemData->ItemEvents)
        {
            Event->OnRemoved(GetOwner());
        }
        OnItemRemoved.Broadcast(Item);
        CleanInvalidItems();
    }
}

bool UInventoryComponent::HasItem(const TSoftObjectPtr<USweetDreamsItem>& ItemData, FInventoryItem& FoundItem) const
{
    if (ItemData.IsValid())
    {
        for (const FInventoryItem& ItemF : Items)
        {
            if (ItemF.ItemData == ItemData)
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

void UInventoryComponent::CleanInvalidItems()
{
    for (int32 i = Items.Num() - 1; i >= 0; --i)
    {
        if (Items.IsValidIndex(i) && Items[i].Count <= 0)
        {
            Items.RemoveAt(i);
        }
    }
}

void UInventoryComponent::OnRep_Items()
{
    //for (const FInventoryItem& NewItem : Items)
    //{
    //    const FInventoryItem* OldItem = PreviousItems.FindByPredicate([&](const FInventoryItem& Item)
    //    {
    //        return Item.ItemData == NewItem.ItemData;
    //    });
    //    if (!OldItem)
    //    {
    //        OnItemAdded.Broadcast(NewItem);
    //    }
    //    else if (OldItem->bIsEquipping != NewItem.bIsEquipping)
    //    {
    //        if (NewItem.bIsEquipping)
    //            OnItemEquipped.Broadcast(NewItem);
    //        else
    //            OnItemUnequiped.Broadcast(NewItem);
    //    }
    //}
    //for (const FInventoryItem& OldItem : PreviousItems)
    //{
    //    const FInventoryItem* NewItem = Items.FindByPredicate([&](const FInventoryItem& Item)
    //    {
    //        return Item.ItemData == OldItem.ItemData;
    //    });
    //    if (!NewItem)
    //    {
    //        OnItemRemoved.Broadcast(OldItem);
    //    }
    //}
    //PreviousItems = Items;
}

