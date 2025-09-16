// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/InventoryItem.h"
#include "Inventory/SweetDreamsItem.h"

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

USweetDreamsItem* UInventoryComponent::GetItemData(UInventoryItem* Item, bool& ValidData)
{
    return Item->GetItemData();
}

int32 UInventoryComponent::AddItem(UInventoryItem*& ItemAdded, USweetDreamsItem* ItemData, int32 Count, bool bAddAsUnique)
{
    int32 Index = -1;
    if (!IsValid(ItemData) || Count <= 0) return Index;
    UInventoryItem* Item = nullptr;
    bool bCreated = false;
    if (!bAddAsUnique && HasItem(ItemData, Item, Index))
    {
        //HasItem filled Item and Index.
    }
    else
    {
        Item = NewObject<UInventoryItem>(this);
        bCreated = true;
    }
    if (IsValid(Item))
    {
        if (bCreated)
        {
            Item->UpdateItemData(ItemData);
            Item->Amount = Count;
            Index = Items.Add(Item);
        }
        else
        {
            Item->Amount += Count;
        }
        Item->OnAdded(GetOwner(), Count);
        ItemAdded = Item;
        OnItemAdded.Broadcast(Item, Index);
    }
    return Index;
}

void UInventoryComponent::UseItem(UPARAM(ref) UInventoryItem*& Item)
{
    if (IsValid(Item))
    {
        Item->OnUsed(GetOwner());
        OnItemUsed.Broadcast(Item);
    }
}

void UInventoryComponent::InspectItem(UPARAM(ref) UInventoryItem*& Item)
{
    if (IsValid(Item))
    {
        Item->OnInspected(GetOwner());
        OnItemInspected.Broadcast(Item);
    }
}

void UInventoryComponent::EquipItem(UPARAM(ref) UInventoryItem*& Item)
{
    if (IsValid(Item))
    {
        Item->bIsBeingEquiped = true;
        Item->OnEquiped(GetOwner());
        OnItemEquipped.Broadcast(Item);
    }
}

void UInventoryComponent::UnequipItem(UPARAM(ref) UInventoryItem*& Item)
{
    if (IsValid(Item))
    {
        Item->bIsBeingEquiped = false;
        Item->OnUnequiped(GetOwner());
        OnItemUnequiped.Broadcast(Item);
    }
}

void UInventoryComponent::RemoveItem(UPARAM(ref) UInventoryItem*& Item, int32 Count)
{
    if (IsValid(Item))
    {
        Item->Amount = FMath::Max(Item->Amount - Count, 0);
        Item->OnRemoved(GetOwner(), Count);
        OnItemRemoved.Broadcast(Item);
        CleanInvalidItems();
    }
}

void UInventoryComponent::RemoveItemAll(UPARAM(ref)UInventoryItem*& Item)
{
    if (IsValid(Item))
    {
        RemoveItem(Item, Item->Amount);
    }
}

bool UInventoryComponent::HasItem(USweetDreamsItem* ItemData, UInventoryItem*& FoundItem, int32& Index) const
{
    if (!IsValid(ItemData)) return false;
    for (int32 i = 0; i < Items.Num(); i++)
    {
        if (IsValid(Items[i]) && Items[i]->GetItemData() == ItemData)
        {
            FoundItem = Items[i];
            Index = i;
            return true;
        }
    }
    return false;
}

UInventoryItem* UInventoryComponent::GetItemByIndex(int32 Index, bool& bFound) const
{
    bFound = false;
    if (Items.Num() > 0 && Items.IsValidIndex(Index))
    {
        bFound = true;
        return Items[Index];
    }
    return nullptr;
}

void UInventoryComponent::CleanInvalidItems()
{
    for (int32 i = Items.Num() - 1; i >= 0; --i)
    {
        if (Items.IsValidIndex(i) && !IsValid(Items[i]) || Items[i]->Amount <= 0)
        {
            Items.RemoveAt(i);
        }
    }
}

