
#include "SweetDreamsAssetFactory.h"

USweetDreamsAssetFactory::USweetDreamsAssetFactory(const FObjectInitializer& objectInitializer)
{
    SupportedClass = UBlueprint::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;

    ParentClass = UObject::StaticClass();
}

UObject* USweetDreamsAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    return NewObject(
        InParent,
        Name,
        Flags
    );
}


