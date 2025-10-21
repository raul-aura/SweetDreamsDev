#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SweetDreamsAssetFactory.generated.h"

UCLASS()
class USweetDreamsGameModeFactory : public UFactory
{
	GENERATED_BODY()

public:
	USweetDreamsGameModeFactory(const FObjectInitializer& objectInitializer);

    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};


