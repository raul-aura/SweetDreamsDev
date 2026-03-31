

#pragma once

#include "CoreMinimal.h"
#include "Data/BattleEvent.h"
#include "BattleEventExecuteElement.generated.h"

class UBattleElementData;

UCLASS()
class SWEETDREAMSBATTLE_API UBattleEventExecuteElement : public UBattleEvent
{
	GENERATED_BODY()

public:

	UBattleEventExecuteElement();

	virtual void ExecuteEvent_Implementation() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Execute Element")
	TObjectPtr<UBattleElementData> BattleElementData;
};
