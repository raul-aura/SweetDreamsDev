// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemEvent.generated.h"

class USweetDreamsItem;

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class SWEETDREAMS_API UItemEvent : public UObject
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
	void OnRemoved(AActor* Owner);

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	USweetDreamsItem* CurrentItemData;
};

