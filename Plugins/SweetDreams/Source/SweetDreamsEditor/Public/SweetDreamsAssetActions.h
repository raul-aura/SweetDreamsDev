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

    virtual FColor GetTypeColor() const override { return FColor(195, 150, 255); }
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

class FAssetTypeActions_SweetDreamsCharacter : public FAssetTypeActions_SweetDreams
{
public:
    FAssetTypeActions_SweetDreamsCharacter(EAssetTypeCategories::Type InCategory)
        : FAssetTypeActions_SweetDreams(InCategory)
    {}

    virtual FText GetName() const override { return LOCTEXT("CharacterName", "Character"); }
    virtual UClass* GetSupportedClass() const override { return ASweetDreamsCharacter::StaticClass(); }
};

class FAssetTypeActions_SweetDreamsPlayerController : public FAssetTypeActions_SweetDreams
{
public:
    FAssetTypeActions_SweetDreamsPlayerController(EAssetTypeCategories::Type InCategory)
        : FAssetTypeActions_SweetDreams(InCategory)
    {}

    virtual FText GetName() const override { return LOCTEXT("PlayerControllerName", "PlayerController"); }
    virtual UClass* GetSupportedClass() const override { return ASweetDreamsPlayerController::StaticClass(); }
};

class FAssetTypeActions_SweetDreamsWidget : public FAssetTypeActions_SweetDreams
{
public:
    FAssetTypeActions_SweetDreamsWidget(EAssetTypeCategories::Type InCategory)
        : FAssetTypeActions_SweetDreams(InCategory)
    {}

    virtual FText GetName() const override { return LOCTEXT("WidgetName", "User Widget"); }
    virtual UClass* GetSupportedClass() const override { return USweetDreamsWidget::StaticClass(); }
};

class FAssetTypeActions_SweetDreamsSaveFile : public FAssetTypeActions_SweetDreams
{
public:
    FAssetTypeActions_SweetDreamsSaveFile(EAssetTypeCategories::Type InCategory)
        : FAssetTypeActions_SweetDreams(InCategory)
    {}

    virtual FText GetName() const override { return LOCTEXT("SaveFileName", "Save File"); }
    virtual UClass* GetSupportedClass() const override { return USweetDreamsSaveFile::StaticClass(); }
};

#undef LOCTEXT_NAMESPACE


