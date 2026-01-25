#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/BattleDataTypes.h"
#include "BattleElement.generated.h"

class UBattleEvent;
class UBattleElementData;

UCLASS(BlueprintType)
class SWEETDREAMSBATTLE_API UBattleElement : public UObject
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "DataClass"))
    static UBattleElement* CreateBattleElement(const UObject* WorldContext, TSubclassOf<UBattleElementData> DataClass);
    
    void Start(/*UBattleContext* Context*/);
    void Tick(float DeltaTime);
    void End();

protected:

    UPROPERTY(BlueprintReadOnly, Category = "Events")
    FBattleElementEventPhase SetupEvents;

    UPROPERTY(BlueprintReadOnly, Category = "Events")
    FBattleElementEventPhase ExecutionEvents;

    UPROPERTY(BlueprintReadOnly, Category = "Events")
    FBattleElementEventPhase EndEvents;

    UPROPERTY(BlueprintReadOnly, Category = "Events")
    EBattleElementPhases CurrentPhase = EBattleElementPhases::Setup;
};

