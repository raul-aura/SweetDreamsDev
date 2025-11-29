// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SweetDreamsItem.generated.h"

class UItemParameters;

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMSITEMSYSTEM_API USweetDreamsItem : public UDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FName ItemUniqueName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FText ItemName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
    FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    UTexture2D* Icon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    UStaticMesh* Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Extra")
    TArray<UItemParameters*> ExtraParameters;
};
