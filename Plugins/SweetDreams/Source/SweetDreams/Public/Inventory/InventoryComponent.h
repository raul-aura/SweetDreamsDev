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
    FInventoryItem()
        :ItemData(nullptr),
        Count(1),
        bIsEquipping(false)
    {}

    FInventoryItem(USweetDreamsItem* InItemData)
        : ItemData(InItemData),
        Count(1),
        bIsEquipping(false)
    {}

    bool IsItemValid() const
    {
        return IsValid(ItemData);
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    USweetDreamsItem* ItemData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Count;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsEquipping;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, FInventoryItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUsed, FInventoryItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInspected, FInventoryItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquipped, FInventoryItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUnequipped, FInventoryItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, FInventoryItem, Item);

UCLASS( ClassGroup = ("SweetDreams"), meta=(BlueprintSpawnableComponent))
class SWEETDREAMS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
    UInventoryComponent();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Inventory")
	static UInventoryComponent* GetInventoryFromActor(const AActor* Actor);
    UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Inventory")
    static USweetDreamsItem* GetItemData(const FInventoryItem& Item, bool& ValidData);
    UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Inventory")
    static bool IsItemValid(const FInventoryItem& Item);
    //
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory", meta = (AdvancedDisplay = 2))
    void AddItem(const TSoftObjectPtr<USweetDreamsItem>& ItemData, int32 Count = 1, bool bAddAsUnique = false);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void UseItem(const FInventoryItem& Item);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void InspectItem(const FInventoryItem& Item);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void EquipItem(UPARAM(ref) FInventoryItem& Item);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void UnequipItem(UPARAM(ref) FInventoryItem& Item);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void RemoveItem(UPARAM(ref) FInventoryItem& Item);
    //
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    bool HasItem(const TSoftObjectPtr<USweetDreamsItem>& ItemData, FInventoryItem& FoundItem) const;
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    FInventoryItem GetItemByIndex(int32 Index, bool& bFound) const;
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void CleanInvalidItems();
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    const TArray<FInventoryItem>& GetItems() const { return Items; }
    //
    UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Inventory")
    FOnItemAdded OnItemAdded;
    UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Inventory")
    FOnItemUsed OnItemUsed;
    UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Inventory")
    FOnItemInspected OnItemInspected;
    UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Inventory")
    FOnItemEquipped OnItemEquipped;
    UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Inventory")
    FOnItemUnequipped OnItemUnequiped;
    UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Core|Inventory")
    FOnItemRemoved OnItemRemoved;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Inventory")
    TArray<FInventoryItem> Items;
};
