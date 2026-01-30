#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/BattleDataTypes.h"
#include "BattleElement.generated.h"

DECLARE_DELEGATE_OneParam(FOnBattleElementDelegate, UBattleElement* /* BattleElement */);

class UBattleEvent;
class UBattleElementData;
class UBattleActorComponent;

UCLASS(BlueprintType, Blueprintable)
class SWEETDREAMSBATTLE_API UBattleElement : public UObject
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element", meta = (WorldContext = "BattleComponent", CallableWithoutWorldContext, DeterminesOutputType = "CustomClass"))
    static UBattleElement* CreateBattleElement(UBattleActorComponent* BattleComponent, UBattleElementData* Data, TSubclassOf<UBattleElement> CustomClass);
    
    void Execute(/*UBattleContext* Context*/);
    void Tick(float DeltaTime);
    void End();

    FOnBattleElementDelegate OnBattleElementEnd;

protected:

    void DuplicateEvents();

    void EvaluatePhases(float DeltaTime);
    void StartCurrentPhase();
    void AdvanceCurrentPhaseEvent();
    void CompleteCurrentPhase();

    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleElementData> BattleElementData = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleActorComponent> Owner = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Events")
    TArray<FBattleElementEventPhase> Phases;
    UPROPERTY(BlueprintReadOnly, Category = "Events")
    FBattleElementEventPhase CurrentPhase;
    UPROPERTY(BlueprintReadOnly, Category = "Events")
    int32 CurrentPhaseIndex = 0;

};

