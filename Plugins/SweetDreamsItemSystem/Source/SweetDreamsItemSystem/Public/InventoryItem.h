#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryItem.generated.h"

class UItemParameters;
class USweetDreamsItem;

UCLASS()
class SWEETDREAMSITEMSYSTEM_API UInventoryItem : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnAdded(AActor* Owner, int32 Count);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnUsed(AActor* Owner);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnInspected(AActor* Owner);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnEquiped(AActor* Owner);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnUnequiped(AActor* Owner);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnRemoved(AActor* Owner, int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams | Core | Inventory")
	USweetDreamsItem* GetItemData() const { return ItemData; }
	void UpdateItemData(USweetDreamsItem* InData) { ItemData = InData; }

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	int32 Amount = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	bool bIsBeingEquiped = false;

protected:
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams | Core | Inventory")
	UItemParameters* GetExtraParameters(int32 Index) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams | Core | Inventory")
	TArray<UItemParameters*> GetAllExtraParameters() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item", meta = (DeterminesOutputType = "Class"))
	UItemParameters* GetExtraParametersOfClass(TSubclassOf<UItemParameters> Class) const;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	TObjectPtr<USweetDreamsItem> ItemData;


};

