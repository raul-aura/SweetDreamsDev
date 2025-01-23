// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "Engine/ObjectLibrary.h"
#include "SweetDreamsItem.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMS_API USweetDreamsItem : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item", meta = (ClampMin = "0"))
	int32 ItemID = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	FText ItemName = FText::FromString("Item");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item", meta = (MultiLine = true))
	FText ItemDescription = FText::FromString("This is an item description.");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	UTexture2D* ItemImage = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	int32 Amount = 0;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
	int32 MaxAmountOrCopies = 999;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	bool bIsStackable = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	bool bIsUsable = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	bool bConsumeWhenUse = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	bool bIsEquippable = true;
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	bool bBeingEquipped = false;

public:
	bool operator==(const USweetDreamsItem& Other) const
	{
		return IsItemEqual(Other);
	}

	bool IsItemEqual(const USweetDreamsItem& Other) const
	{
		return ItemID == Other.ItemID;
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCreateItem();
	virtual void OnCreateItem_Implementation() { ; }
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnAddItem();
	virtual void OnAddItem_Implementation() { ; }
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnRemoveItem();
	virtual void OnRemoveItem_Implementation() { ; }
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUseItem();
	virtual void OnUseItem_Implementation() { ; }
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEquipItem();
	virtual void OnEquipItem_Implementation() { ; }
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUnequipItem();
	virtual void OnUnequipItem_Implementation() { ; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Core|Item")
	FText GetItemName() const;
	FText GetItemName_Implementation() const { return ItemName; }
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Core|Item")
	FText GetItemDescription() const;
	FText GetItemDescription_Implementation() const { return ItemDescription; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	bool IsStackable() const { return bIsStackable; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	bool IsConsumable() const { return bConsumeWhenUse; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	bool IsBeingEquipped() const { return bBeingEquipped; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	bool IsUsable() const { return bIsUsable && Amount > 0; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	bool IsEquippable() const { return bIsEquippable; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	void SetBeingEquipped(bool bValue = true) { bBeingEquipped = bValue; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	void SetItemOwner(AActor* NewOwner);
	UFUNCTION(BlueprintCallable)
	AActor* GetItemOwner() const { return ItemOwner; }
	UPROPERTY(BlueprintReadOnly)
	AActor* ItemOwner = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	void IncreaseAmount();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	void DecreaseAmount();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	int32 GetAmount() const { return Amount; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Item")
	int32 GetMaxAmountOrCopies() const { return MaxAmountOrCopies; }
};
