

#pragma once

#include "CoreMinimal.h"
#include "Data/BattleEvent.h"
#include "BattleEventRandomizer.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API UBattleEventRandomizer : public UBattleEvent
{
	GENERATED_BODY()

public:

	UBattleEventRandomizer();

	virtual void OnEventCreated_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Battle Event Randomizer")
	bool IsElementRandomizable() const;
	virtual bool IsElementRandomizable_Implementation() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Battle Event Randomizer")
	bool IsExecutionForced() const;
	virtual bool IsExecutionForced_Implementation() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Battle Event Randomizer")
	float GetWeight() const;
	virtual float GetWeight_Implementation() const;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Randomizer")
	float BaseWeight = 1.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Randomizer", meta = (ClampMin = 0, UIMin = 0))
	int32 MinExecutions = 0;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Randomizer")
	int32 MaxExecutions = 0;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Randomizer", meta = (ClampMin = 0, UIMin = 0, ClampMax = 100, UIMax = 100))
	float HealthRangeMin = 0.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Randomizer", meta = (ClampMin = 0, UIMin = 0, ClampMax = 100, UIMax = 100))
	float HealthRangeMax = 100.f;

private:

	void IncrementExecution(UBattleActorComponent* BattleActor, UBattleElement* Element);

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 TimesExecuted = 0;
};
