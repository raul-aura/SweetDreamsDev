

#pragma once

#include "CoreMinimal.h"
#include "Data/BattleEvent.h"
#include "BattleEventCreateElement.generated.h"

class UBattleElementData;
class UBattleElement;
class UBattleContext;

UCLASS()
class SWEETDREAMSBATTLE_API UBattleEventCreateElement : public UBattleEvent
{
	GENERATED_BODY()

public:

	virtual void ExecuteEvent_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle|Battle Event Create Element")
	TArray<UBattleActorComponent*> GetElementTargets();

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle|Battle Event Create Element")
	void OnBattleElementCreated(UBattleElement* Element);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Execute Element")
	TObjectPtr<UBattleElementData> BattleElementData;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Execute Element")
	TSubclassOf<UBattleElement> CustomClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Execute Element")
	TSubclassOf<UBattleContext> CustomContextClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Execute Element")
	bool bShouldUnregisterOnEnd = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Execute Element")
	bool bAutoExecute = false;
};

