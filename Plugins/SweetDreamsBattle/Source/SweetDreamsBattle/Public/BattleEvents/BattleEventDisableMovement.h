

#pragma once

#include "CoreMinimal.h"
#include "Data/BattleEvent.h"
#include "BattleEventDisableMovement.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API UBattleEventDisableMovement : public UBattleEvent
{
	GENERATED_BODY()

public:

	virtual void ExecuteEvent_Implementation() override;
	virtual bool IsFinished_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Event Disable Movement")
	void ReenableMovement();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Disable Movement")
	bool bUseTimedDuration = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Disable Movement", meta = (EditCondition = "bUseTimedDuration"))
	float Duration = 1.f;

private:

	TArray<UBattleActorComponent*> CachedTargets;

	FTimerHandle ReenableTimer;
	bool bTimerExecuted = false;
};
