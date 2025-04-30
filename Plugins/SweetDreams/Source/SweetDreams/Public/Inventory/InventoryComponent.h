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
    }

    USweetDreamsItem* GetItemData() const
    {
        if (!ItemData && ItemDataPtr.IsValid())
        {
            ItemData = ItemDataPtr.LoadSynchronous();
        }
        return ItemData;
    }

    bool IsValid() const { return ItemDataPtr.IsValid(); }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (DisplayName = "Item Data"))
    TSoftObjectPtr<USweetDreamsItem> ItemDataPtr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Count = 1;

private:
    mutable USweetDreamsItem* ItemData;
};

UCLASS( ClassGroup = ("SweetDreams"), meta=(BlueprintSpawnableComponent))
class SWEETDREAMS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
    UInventoryComponent();

	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Inventory")
	static UInventoryComponent* GetInventoryFromActor(const AActor* Actor);
    UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Inventory")
    static USweetDreamsItem* GetItemData(const FInventoryItem& Item);

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory", meta = (AdvancedDisplay = 2))
    void AddItem(const TSoftObjectPtr<USweetDreamsItem>& ItemData, int32 Count = 1, bool bAddAsUnique = false);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void UseItem(const FInventoryItem& Item);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void InspectItem(const FInventoryItem& Item);
    //





    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    bool HasItem(const TSoftObjectPtr<USweetDreamsItem>& ItemData, FInventoryItem& FoundItem) const;
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    FInventoryItem GetItemByIndex(int32 Index, bool& bFound) const;
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    const TArray<FInventoryItem>& GetItems() const { return Items; }

protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FInventoryItem> Items;
};
