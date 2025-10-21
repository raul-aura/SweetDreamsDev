
#include "SweetDreamsAssetFactory.h"
#include "AssetActionsHeaders.h"

#include "KismetCompilerModule.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "BlueprintEditorModule.h"

USweetDreamsGameModeFactory::USweetDreamsGameModeFactory(const FObjectInitializer& objectInitializer)
{
	SupportedClass = ASweetDreamsGameMode::StaticClass();
	bCreateNew = true;
    bEditAfterNew = true;
}

UObject* USweetDreamsGameModeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    return FKismetEditorUtilities::CreateBlueprint(ASweetDreamsGameMode::StaticClass(), InParent, Name, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}
