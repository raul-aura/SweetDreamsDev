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

    void Initialize(UBattleElement* Owner, TObjectPtr<UBattleActorComponent> InInstigator, TArray<UBattleActorComponent*> InTargets);

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    UBattleElement* GetOwnerElement() const;

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void Damage(float Value);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void Heal(float Value);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void Kill();
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void Ressurect();
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void RequestBattleElementEnd();
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    float GetDeltaTime() const;

    // start animation, start sequence, create particle
    // play sound, trigger another event, start dialogue

protected:

    UPROPERTY(BlueprintReadOnly, Category = "Battle Context")
    TObjectPtr<UBattleActorComponent> Instigator;
    UPROPERTY(BlueprintReadOnly, Category = "Battle Context")
    TArray<TObjectPtr<UBattleActorComponent>> Targets;
    UPROPERTY()
    TWeakObjectPtr<UBattleElement> OwnerElement;

    FORCEINLINE UBattleElement* GetOwner() const
    {
        return OwnerElement.Get();
    }

};

