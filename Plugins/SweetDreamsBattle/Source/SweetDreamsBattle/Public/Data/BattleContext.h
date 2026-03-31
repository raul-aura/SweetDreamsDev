#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/BattleDataTypes.h"
#include "BattleContext.generated.h"

class UBattleElement;
class UBattleActorComponent;

UCLASS(BlueprintType, Blueprintable)
class SWEETDREAMSBATTLE_API UBattleContext : public UObject
{
	GENERATED_BODY()

public:

    void Initialize(UBattleElement* Owner, TObjectPtr<UBattleActorComponent> InInstigator, TArray<UBattleActorComponent*> InCandidates);
    
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void UpdateCandidates(TArray<UBattleActorComponent*> InCandidates);

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    UBattleElement* GetOwnerElement() const;
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    UBattleActorComponent* GetInstigator() const;
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    TArray<UBattleActorComponent*> GetSelectedTargets(const FSelectedTargetsSettings& Settings) const;

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    FDamageHealResult Damage(float Value, FGameplayTagContainer EffectTags, const FSelectedTargetsSettings& Settings);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    FDamageHealResult Heal(float Value, FGameplayTagContainer EffectTags, const FSelectedTargetsSettings& Settings);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void Kill(const FSelectedTargetsSettings& Settings);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void Ressurect(const FSelectedTargetsSettings& Settings);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    FBattleParameterModifier AddModifierToParameter(FGameplayTag ParameterTag, EParameterModifierType ModifierType, float ModifierValue, const FSelectedTargetsSettings& Settings);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void RemoveModifierFromParameter(FGameplayTag ParameterTag, const FBattleParameterModifier& Modifier, const FSelectedTargetsSettings& Settings);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void IncreaseParameterResource(FGameplayTag ParameterTag, float Value, const FSelectedTargetsSettings& Settings);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void DecreaseParameterResource(FGameplayTag ParameterTag, float Value, const FSelectedTargetsSettings& Settings);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    AActor* SpawnActor(TSubclassOf<AActor> ActorClass, FTransform Transform);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    void RequestBattleElementEnd();
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    float GetDeltaTime() const;

protected:

    UPROPERTY(BlueprintReadOnly, Category = "Battle Context")
    TObjectPtr<UBattleActorComponent> Instigator = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "Battle Context")
    TArray<TObjectPtr<UBattleActorComponent>> CandidateActors;
    UPROPERTY()
    TWeakObjectPtr<UBattleElement> OwnerElement;

    FORCEINLINE UBattleElement* GetOwner() const
    {
        return OwnerElement.Get();
    }

};

