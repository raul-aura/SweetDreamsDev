// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	AddStartingItems();
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

void UInventoryComponent::AddStartingItems()
{
	if (StartingItems.Num() > 0)
	{
		for (TSubclassOf<USweetDreamsItem> ItemClass : StartingItems)
		{
			ObtainItem(CreateItem(ItemClass));
		}
	}
}

USweetDreamsItem* UInventoryComponent::CreateItem(TSubclassOf<USweetDreamsItem> Item)
{
	USweetDreamsItem* NewItem = nullptr;
	if (IsValid(Item))
	{
		NewItem = NewObject<USweetDreamsItem>(this, Item);
		if (IsValid(NewItem))
		{
			NewItem->OnCreateItem();
		}
	}
	return NewItem;
}

void UInventoryComponent::ObtainItem(USweetDreamsItem* Item)
{
	if (IsValid(Item))
	{
		if (ObtainedItems.Contains(Item))
		{
			if (Item->IsStackable())
			{
				Item->IncreaseAmount();
				Item->OnAddItem();
				return;
			}
			else
			{
				if (HasReachedMaxCopies(Item->GetClass())) return;
				USweetDreamsItem* NewItem = CreateItem(Item->GetClass());
				if (IsValid(NewItem))
				{
					NewItem->IncreaseAmount();
					NewItem->OnAddItem();
					ObtainedItems.Add(NewItem);
					return;
				}
			}
		}
		Item->SetItemOwner(GetOwner());
		Item->IncreaseAmount();
		Item->OnAddItem();
		ObtainedItems.Add(Item);
	}
}

bool UInventoryComponent::UseItem(USweetDreamsItem* Item, bool bEquipOnUse)
{
	if (IsValid(Item) && Item->IsUsable())
	{
		Item->OnUseItem();
		if (Item->IsConsumable())
		{
			Item->DecreaseAmount();
			if (Item->GetAmount() <= 0 && ObtainedItems.Num() > 0)
			{
				RemoveItem(Item);
			}
		}
		if (Item->IsEquippable() && bEquipOnUse)
		{
			EquipItem(Item);
		}
		return true;
	}
	return false;
}

bool UInventoryComponent::EquipItem(USweetDreamsItem* Item)
{
	if (IsValid(Item) && Item->IsEquippable() && !Item->IsBeingEquipped() && !EquippedItems.Contains(Item))
	{
		Item->SetBeingEquipped(true);
		Item->OnEquipItem();
		EquippedItems.Add(Item);
		return true;
	}
	return false;
}

bool UInventoryComponent::UnequipItem(USweetDreamsItem* Item)
{
	if (IsValid(Item) && EquippedItems.Contains(Item))
	{
		Item->SetBeingEquipped(false);
		Item->OnUnequipItem();
		EquippedItems.Remove(Item);
		return true;
	}
	return false;
}

void UInventoryComponent::RemoveItem(USweetDreamsItem* Item)
{
	if (!IsValid(Item) || !ObtainedItems.Contains(Item)) return;
	Item->OnRemoveItem();
	ObtainedItems.Remove(Item);
	if (EquippedItems.Contains(Item))
	{
		Item->OnUnequipItem();
		EquippedItems.Remove(Item);
	}
	Item->ConditionalBeginDestroy();
}

bool UInventoryComponent::HasReachedMaxCopies(TSubclassOf<USweetDreamsItem> ItemClass) const
{
	int32 Count = 0;
	for (USweetDreamsItem* Item : ObtainedItems)
	{
		if (IsValid(Item) && Item->IsA(ItemClass))
		{
			Count++;
			if (Count >= Item->GetMaxAmountOrCopies())
			{
				return true;
			}
		}
	}
	return false;
}

