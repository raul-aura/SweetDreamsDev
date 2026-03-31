

#pragma once

#include "CoreMinimal.h"
#include "Data/BattleEvent.h"
#include "BattleEventHeal.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API UBattleEventHeal : public UBattleEvent
{
	GENERATED_BODY()

public:

	UBattleEventHeal();

	virtual void ExecuteEvent_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Battle Event Heal")
	float GetExpectedHealAmount() const;
	virtual float GetExpectedHealAmount_Implementation() const;

	// The healing will be calculated based on the sum of a percentage of the defined parameters.
	// Percentage is a normalized value, meaning 1.0 equals to 100% of the parameter value.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Heal", meta = (ForceInlineRow))
	TMap<FGameplayTag, float> HealValueParameters;

	// This is for the designer to evaluate the types of healing and to multiply the healing by
	// the parameter value equal to these tags.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Heal", meta = (ForceInlineRow))
	FGameplayTagContainer HealTags;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Heal")
	bool bUseFixedAmount = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Heal", meta = (EditCondition = "bUseFixedAmount"))
	float FixedAmount = 0.f;
};
