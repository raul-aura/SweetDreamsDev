#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryItem.generated.h"

class UItemParameters;
class USweetDreamsItem;

UCLASS(BlueprintType)
class SWEETDREAMSITEMSYSTEM_API UInventoryItem : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnAdded(AActor* Owner, int32 Count);
	virtual void OnAdded_Implementation(AActor* Owner, int32 Count) {}
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnUsed(AActor* Owner);
	virtual void OnUsed_Implementation(AActor* Owner) {}
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnInspected(AActor* Owner);
	virtual void OnInspected_Implementation(AActor* Owner) {}
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnEquiped(AActor* Owner);
	virtual void OnEquiped_Implementation(AActor* Owner) {}
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnUnequiped(AActor* Owner);
	virtual void OnUnequiped_Implementation(AActor* Owner) {}
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams | Core | Inventory")
	void OnRemoved(AActor* Owner, int32 Count);
	virtual void OnRemoved_Implementation(AActor* Owner, int32 Count) {}

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams | Core | Inventory")
	USweetDreamsItem* GetItemData() const { return ItemData; }
	void UpdateItemData(USweetDreamsItem* InData) { ItemData = InData; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams | Core | Inventory")
	UItemParameters* GetExtraParameters(int32 Index) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams | Core | Inventory")
	TArray<UItemParameters*> GetAllExtraParameters() const;
	UFUNCTION(BlueprintCallable, Category = "Item", meta = (DeterminesOutputType = "Class"))
	UItemParameters* GetExtraParametersOfClass(TSubclassOf<UItemParameters> Class) const;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	int32 Amount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	bool bIsBeingEquiped = false;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	TObjectPtr<USweetDreamsItem> ItemData;
};

