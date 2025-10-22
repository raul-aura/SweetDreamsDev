#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SweetDreamsAssetFactory.generated.h"

UCLASS()
class USweetDreamsGameModeFactory : public UFactory
{
	GENERATED_BODY()

public:
	USweetDreamsGameModeFactory(const FObjectInitializer& ObjectInitializer);

    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class USweetDreamsCharacterFactory : public UFactory
{
	GENERATED_BODY()

public:
	USweetDreamsCharacterFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class USweetDreamsPlayerControllerFactory : public UFactory
{
	GENERATED_BODY()

public:
	USweetDreamsPlayerControllerFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class USweetDreamsWidgetFactory : public UFactory
{
	GENERATED_BODY()

public:
	USweetDreamsWidgetFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class USweetDreamsSaveFileFactory : public UFactory
{
	GENERATED_BODY()

public:
	USweetDreamsSaveFileFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

