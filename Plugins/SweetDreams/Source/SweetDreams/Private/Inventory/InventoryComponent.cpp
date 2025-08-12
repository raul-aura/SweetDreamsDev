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

void UInventoryComponent::AddItem(USweetDreamsItem* ItemData, int32 Count, bool bAddAsUnique)
{
    if (!IsValid(ItemData) || Count <= 0) return;
    int32 Index;
    FInventoryItem ExistingItem;
    if (!bAddAsUnique && HasItem(ItemData, ExistingItem, Index))
    {
        Items[Index].Count += Count;
        for (auto Event : Items[Index].ItemData->ItemEvents)
        {
            Event->OnAdded(GetOwner(), Count);
        }
        OnItemAdded.Broadcast(Items[Index], Index);
        return;
    }
    FInventoryItem NewItem(ItemData);
    NewItem.ItemData->AssignItemToEvents();
    for (auto Event : NewItem.ItemData->ItemEvents)
    {
        Event->OnAdded(GetOwner(), Count);
    }
    Index = Items.Add(NewItem);
    OnItemAdded.Broadcast(NewItem, Index);
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

bool UInventoryComponent::HasItem(USweetDreamsItem* ItemData, FInventoryItem& FoundItem, int32& Index) const
{
    if (!IsValid(ItemData)) return false;
    for (int32 i = 0; i < Items.Num(); i++)
    {
        if (Items[i].ItemData == ItemData)
        {
            FoundItem = Items[i];
            Index = i;
            return true;
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

