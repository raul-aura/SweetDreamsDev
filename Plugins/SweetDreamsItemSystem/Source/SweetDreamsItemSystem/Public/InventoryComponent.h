// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryItem;
class USweetDreamsItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAdded, UInventoryItem*, Item, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUsed, UInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInspected, UInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquipped, UInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUnequipped, UInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, UInventoryItem*, Item);

UCLASS( ClassGroup = ("SweetDreams"), meta=(BlueprintSpawnableComponent))
class SWEETDREAMSITEMSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
    UInventoryComponent();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Inventory")
	static UInventoryComponent* GetInventoryFromActor(const AActor* Actor);
    UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Inventory")
    static USweetDreamsItem* GetItemData(UInventoryItem* Item, bool& ValidData);
    //
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory", meta = (AdvancedDisplay = 2))
    int32 AddItem(UInventoryItem*& ItemAdded, USweetDreamsItem* ItemData, int32 Count = 1, bool bAddAsUnique = false);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void UseItem(UPARAM(ref) UInventoryItem*& Item);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void InspectItem(UPARAM(ref) UInventoryItem*& Item);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void EquipItem(UPARAM(ref) UInventoryItem*& Item);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void UnequipItem(UPARAM(ref) UInventoryItem*& Item);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void RemoveItem(UPARAM(ref) UInventoryItem*& Item, int32 Count = 1);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void RemoveItemAll(UPARAM(ref) UInventoryItem*& Item);
    //
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    bool HasItem(USweetDreamsItem* ItemData, UInventoryItem*& FoundItem, int32& Index) const;
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    UInventoryItem* GetItemByIndex(int32 Index, bool& bFound) const;
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    void CleanInvalidItems();
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
    const TArray<UInventoryItem*> GetItems() const { return Items; }
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
    TArray<UInventoryItem*> Items;
};
