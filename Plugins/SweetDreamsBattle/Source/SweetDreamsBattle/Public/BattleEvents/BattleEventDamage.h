

#pragma once

#include "CoreMinimal.h"
#include "Data/BattleEvent.h"
#include "BattleEventDamage.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API UBattleEventDamage : public UBattleEvent
{
	GENERATED_BODY()

public:
	
	UBattleEventDamage();

	virtual void ExecuteEvent_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Event Damage")
	float GetExpectedDamageAmount() const;

	// The damage will be calculated based on the sum of a percentage of the defined parameters.
	// Percentage is a normalized value, meaning 1.0 equals to 100% of the parameter value.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damage", meta = (ForceInlineRow))
	TMap<FGameplayTag, float> DamageValueParameters;

	// This is for the designer to evaluate the types of damage and to multiply the damage by
	// the parameter value equal to these tags.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damage", meta = (ForceInlineRow))
	FGameplayTagContainer DamageTags;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damage")
	bool bUseFixedAmount = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damage", meta = (EditCondition="bUseFixedAmount"))
	float FixedAmount = 0.f;
};

