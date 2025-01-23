// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SweetDreamsItem.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWEETDREAMS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Core|Inventory")
	static UInventoryComponent* GetInventoryFromActor(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
	void AddStartingItems();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
	USweetDreamsItem* CreateItem(TSubclassOf<USweetDreamsItem> Item);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
	void ObtainItem(USweetDreamsItem* Item);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
	bool UseItem(USweetDreamsItem* Item, bool bEquipOnUse = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
	bool EquipItem(USweetDreamsItem* Item);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
	bool UnequipItem(USweetDreamsItem* Item);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
	void RemoveItem(USweetDreamsItem* Item);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
	TArray<USweetDreamsItem*> GetObtainedItems() const { return ObtainedItems; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
	TArray<USweetDreamsItem*> GetEquippedItems() const { return EquippedItems; }

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Inventory")
	bool HasReachedMaxCopies(TSubclassOf<USweetDreamsItem> ItemClass) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TArray<TSubclassOf<USweetDreamsItem>> StartingItems;
	UPROPERTY(BlueprintReadWrite, Category = "Items")
	TArray<USweetDreamsItem*> ObtainedItems;
	UPROPERTY(BlueprintReadWrite, Category = "Items")
	TArray<USweetDreamsItem*> EquippedItems;
};
