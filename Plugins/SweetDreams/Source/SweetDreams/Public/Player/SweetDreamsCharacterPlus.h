

#pragma once

#include "CoreMinimal.h"
#include "Player/SweetDreamsCharacter.h"
#include "SweetDreamsCharacterPlus.generated.h"

class UMulticameraComponent;

UCLASS()
class SWEETDREAMS_API ASweetDreamsCharacterPlus : public ASweetDreamsCharacter
{
	GENERATED_BODY()

public:

	ASweetDreamsCharacterPlus();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UMulticameraComponent* MulticameraComponent;

	
	
};
