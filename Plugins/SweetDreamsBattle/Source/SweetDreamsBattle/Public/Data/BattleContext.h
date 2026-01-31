#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleContext.generated.h"

class UBattleElement;
class UBattleActorComponent;

UCLASS(BlueprintType, Blueprintable)
class SWEETDREAMSBATTLE_API UBattleContext : public UObject
{
	GENERATED_BODY()

public:

    UPROPERTY()
    TObjectPtr<UBattleActorComponent> Instigator;

    UPROPERTY()
    TArray<TObjectPtr<UBattleActorComponent>> Targets;

    UPROPERTY()
    TWeakObjectPtr<UBattleElement> OwnerElement;

    // damage, heal, add state, remove state, restore mana, remove mana, kill, revive, set ability to act, start animation, start sequence, create particle
    // play sound, trigger another event, start dialogue
    // get delta time from element
    // request end battle element
};

