#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/BattleDataTypes.h"
#include "BattleElement.generated.h"

DECLARE_DELEGATE_OneParam(FOnBattleElementDelegate, UBattleElement* /* BattleElement */);

class UBattleEvent;
class UBattleElementData;
class UBattleActorComponent;
class UBattleContext;

UCLASS(BlueprintType, Blueprintable)
class SWEETDREAMSBATTLE_API UBattleElement : public UObject
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element", meta = (WorldContext = "BattleComponent", CallableWithoutWorldContext, DeterminesOutputType = "CustomClass"))
    static UBattleElement* CreateBattleElement(UBattleActorComponent* BattleComponent, TArray<UBattleActorComponent*> Targets, UBattleElementData* Data, TSubclassOf<UBattleElement> CustomClass, TSubclassOf<UBattleContext> CustomContextClass, bool bAutoExecute = false);
    
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    void Execute();
    void Tick(float DeltaTime);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    void End();

    FOnBattleElementDelegate OnBattleElementEnd;

    float CachedDeltaTime = 0.f;

protected:

    void CreateBattleContext(TArray<UBattleActorComponent*> InTargets, TSubclassOf<UBattleContext> CustomContextClass);
    void DuplicateEvents();

    void EvaluateEvents(float DeltaTime);
    void StartCurrentEvent();
    void AdvanceEvent();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EBattleElementEndMode EndMode = EBattleElementEndMode::Auto;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleElementData> BattleElementData = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleActorComponent> Owner = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleContext> BattleContext = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    bool bElementInExecution = false;

    UPROPERTY(BlueprintReadOnly, Category = "Events")
    TArray<TObjectPtr<UBattleEvent>> Events;
    UPROPERTY(BlueprintReadOnly, Category = "Events")
    TObjectPtr<UBattleEvent> CurrentEvent;
    UPROPERTY(BlueprintReadOnly, Category = "Events")
    int32 CurrentEventIndex = 0;
};

