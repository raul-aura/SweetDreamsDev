#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/BattleDataTypes.h"
#include "Battle/SweetDreamsBattleManager.h"
#include "SweetDreamsBattleInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USweetDreamsBattleInterface : public UInterface
{
	GENERATED_BODY()
};

/*
Interface to communicate with other actors that are not native to the Sweet Dreams Battle plugin,
but may want to react with the plugin functionality or even extend it.
*/
class SWEETDREAMSBATTLE_API ISweetDreamsBattleInterface
{
	GENERATED_BODY()

public:

	// Sends a Battle Element to the actor with the Sweet Dreams Battle interface.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void SendBattleElement(UBattleElement* BattleElement);

	// Sends a Battle Actor Component to the actor with the Sweet Dreams Battle interface.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Sweet Dreams|Battle")
	void SendBattleActorComponent(UBattleActorComponent* BattleActorComponent);

};

