
#include "SweetDreamsAssetFactory.h"
#include "AssetActionsHeaders.h"

#include "KismetCompilerModule.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "BlueprintEditorModule.h"
#include "WidgetBlueprint.h"

USweetDreamsGameModeFactory::USweetDreamsGameModeFactory(const FObjectInitializer& ObjectInitializer)
{
	SupportedClass = ASweetDreamsGameMode::StaticClass();
	bCreateNew = true;
    bEditAfterNew = true;
}

UObject* USweetDreamsGameModeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    return FKismetEditorUtilities::CreateBlueprint(ASweetDreamsGameMode::StaticClass(), InParent, Name, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}

USweetDreamsCharacterFactory::USweetDreamsCharacterFactory(const FObjectInitializer& ObjectInitializer)
{
	SupportedClass = ASweetDreamsCharacter::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* USweetDreamsCharacterFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(ASweetDreamsCharacter::StaticClass(), InParent, Name, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}

USweetDreamsPlayerControllerFactory::USweetDreamsPlayerControllerFactory(const FObjectInitializer& ObjectInitializer)
{
	SupportedClass = ASweetDreamsPlayerController::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* USweetDreamsPlayerControllerFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(ASweetDreamsPlayerController::StaticClass(), InParent, Name, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}

USweetDreamsWidgetFactory::USweetDreamsWidgetFactory(const FObjectInitializer& ObjectInitializer)
{
	SupportedClass = USweetDreamsWidget::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* USweetDreamsWidgetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(USweetDreamsWidget::StaticClass(), InParent, Name, BPTYPE_Normal, UWidgetBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}

USweetDreamsSaveFileFactory::USweetDreamsSaveFileFactory(const FObjectInitializer& ObjectInitializer)
{
	SupportedClass = USweetDreamsSaveFile::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* USweetDreamsSaveFileFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(USweetDreamsSaveFile::StaticClass(), InParent, Name, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}
