#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BattleParameterData.generated.h"

class UBattleParameterEvent;

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMSBATTLE_API UBattleParameterData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameter")
	FText DisplayText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parameter")
	float InitialValue = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parameter", meta = (InlineEditConditionToggle))
	bool bUseMinValue = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parameter", meta = (EditCondition="bUseMinValue"))
	float MinValue = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parameter", meta = (InlineEditConditionToggle))
	bool bUseMaxValue = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parameter", meta = (EditCondition = "bUseMaxValue"))
	float MaxValue = 1.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parameter")
	bool bIsResource = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parameter", meta = (EditCondition = "bIsResource"))
	bool bPreserveResourceRatioOnUpdate = true;

	// A normalized value that represents the initial value of the Resource Value of this parameter, based on the Parameter Value.
	//
	// Ratio 1.0 = Parameter Value current value
	// Ratio 0.0 = Parameter Value min value, 0 if not specified.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parameter", meta = (EditCondition = "bIsResource", ClampMin=0, UIMin=0, ClampMax=1, UIMax=1))
	float InitialResourceValueRatio = 1.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced, Category = "Events")
	TObjectPtr<UBattleParameterEvent> Event;
};

