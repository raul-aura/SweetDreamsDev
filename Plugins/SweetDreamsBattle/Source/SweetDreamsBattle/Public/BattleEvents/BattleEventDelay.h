

#pragma once

#include "CoreMinimal.h"
#include "Data/BattleEvent.h"
#include "BattleEventDelay.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API UBattleEventDelay : public UBattleEvent
{
	GENERATED_BODY()

public:

	virtual void ExecuteEvent_Implementation() override;
	virtual bool IsFinished_Implementation() const override;
	virtual void Tick_Implementation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Event Delay")
	float Duration = 1.f;

private:

	float TimeElapsed = 0.f;

};
