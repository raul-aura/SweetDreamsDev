

#include "Player/SweetDreamsCharacterPlus.h"
#include "Player/MulticameraComponent.h"

ASweetDreamsCharacterPlus::ASweetDreamsCharacterPlus()
{
	MulticameraComponent = CreateDefaultSubobject<UMulticameraComponent>("Multicamera Component");
	AddOwnedComponent(MulticameraComponent);
}



