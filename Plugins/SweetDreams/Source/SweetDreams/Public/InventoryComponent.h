// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SweetDreamsItem.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

public:
    FInventoryItem() = default;

    explicit FInventoryItem(const TSoftObjectPtr<USweetDreamsItem>& InItemData)
        : ItemDataPtr(InItemData)
        , ItemData(nullptr)
    {        
        if (USweetDreamsItem* Data = GetItemData())
        {
            ItemName = Data->ItemUniqueName;
        }
    }

    USweetDreamsItem* GetItemData() const
    {
        if (!ItemData && ItemDataPtr.IsValid())
        {
            ItemData = ItemDataPtr.LoadSynchronous();
            if (ItemData)
            {
                ItemName = ItemData->ItemUniqueName;
            }
        }
        return ItemData;
    }

    bool IsValid() const { return ItemDataPtr.IsValid(); }

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    mutable FName ItemName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSoftObjectPtr<USweetDreamsItem> ItemDataPtr;

private:
    mutable USweetDreamsItem* ItemData;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWEETDREAMS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
    UInventoryComponent();

	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Inventory")
	static UInventoryComponent* GetInventoryFromActor(const AActor* Actor);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddItem(const TSoftObjectPtr<USweetDreamsItem>& Item);
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UseItem(int32 Index);
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void InspectItem(int32 Index);
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UseItemByName(FName ItemName);
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void InspectItemByName(FName ItemName);
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 GetItemCountByName(FName ItemName) const;
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool HasItem(FName ItemName) const;
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 FindItemIndex(FName ItemName) const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    const TArray<FInventoryItem>& GetItems() const { return Items; }

protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FInventoryItem> Items;
};
