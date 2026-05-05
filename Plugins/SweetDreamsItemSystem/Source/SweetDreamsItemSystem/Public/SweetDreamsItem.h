// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SweetDreamsItem.generated.h"

class UItemParameters;

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMSITEMSYSTEM_API USweetDreamsItem : public UDataAsset
{
	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FGameplayTag Identifier;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FText DisplayText;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
    FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    TObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    TObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Extra")
    TArray<TObjectPtr<UItemParameters>> ExtraParameters;
};
