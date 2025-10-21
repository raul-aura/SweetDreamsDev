#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "AssetActionsHeaders.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

class FAssetTypeActions_SweetDreams : public FAssetTypeActions_Base
{
public:
    FAssetTypeActions_SweetDreams(EAssetTypeCategories::Type InCategory)
        : SweetDreamsCategory(InCategory) {}

    virtual FColor GetTypeColor() const override { return FColor(200, 60, 60); }
    virtual uint32 GetCategories() override { return SweetDreamsCategory; }
    virtual const TArray<FText>& GetSubMenus() const override
    {
        static const TArray<FText> SubMenus
        {
            LOCTEXT("SubMenu_Core", "Core"),
        };
        return SubMenus;
    }

protected:
    EAssetTypeCategories::Type SweetDreamsCategory;
};

class FAssetTypeActions_SweetDreamsGameMode : public FAssetTypeActions_SweetDreams
{
public:
    FAssetTypeActions_SweetDreamsGameMode(EAssetTypeCategories::Type InCategory)
        : FAssetTypeActions_SweetDreams(InCategory)
    {}

    virtual FText GetName() const override { return LOCTEXT("GameModeName", "Game Mode"); }
    virtual UClass* GetSupportedClass() const override { return ASweetDreamsGameMode::StaticClass(); }
};

#undef LOCTEXT_NAMESPACE


