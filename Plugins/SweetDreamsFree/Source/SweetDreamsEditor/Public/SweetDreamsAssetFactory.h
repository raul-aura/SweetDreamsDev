#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SweetDreamsAssetFactory.generated.h"

UCLASS()
class USweetDreamsAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	USweetDreamsAssetFactory(const FObjectInitializer& objectInitializer);

    UPROPERTY(EditAnywhere, Category = "SweetDreams")
    TSubclassOf<UObject> ParentClass;

    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

