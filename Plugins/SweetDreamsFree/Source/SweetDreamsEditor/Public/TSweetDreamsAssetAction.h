#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "Factories/BlueprintFactory.h"

class SweetDreamsAssetAction : public FAssetTypeActions_Base
{
public:
    explicit SweetDreamsAssetAction(EAssetTypeCategories::Type InCategory, TSubclassOf<UObject> InClass)
        : AssetCategory(InCategory),
        AssetClass(InClass)
    {

    }

    // interface

    virtual FText GetName() const override { return FText::FromString(TEXT("Sweet Dreams Asset")); }
    virtual FColor GetTypeColor() const override { return FColor(100, 180, 255); }
    virtual UClass* GetSupportedClass() const override { return AssetClass; }
    virtual uint32 GetCategories() override { return AssetCategory; }
	
private:
    EAssetTypeCategories::Type AssetCategory;
    TSubclassOf<UObject> AssetClass;
};

