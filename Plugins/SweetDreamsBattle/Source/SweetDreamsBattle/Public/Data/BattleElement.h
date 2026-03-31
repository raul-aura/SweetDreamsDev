#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/BattleDataTypes.h"
#include "BattleElement.generated.h"

DECLARE_DELEGATE_OneParam(FOnBattleElementDelegate, UBattleElement* /* BattleElement */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleElementDelegateBP);

class UBattleEvent;
class UBattleElementData;
class UBattleActorComponent;
class UBattleContext;

UCLASS(BlueprintType, Blueprintable)
class SWEETDREAMSBATTLE_API UBattleElement : public UObject
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element", meta = (DeterminesOutputType = "CustomClass", AdvancedDisplay = 2, AutoCreateRefTerm = "Targets"))
    static UBattleElement* CreateBattleElement(UBattleActorComponent* BattleComponent, UBattleElementData* Data, const TArray<UBattleActorComponent*>& Targets, TSubclassOf<UBattleElement> CustomClass, TSubclassOf<UBattleContext> CustomContextClass, bool bShouldUnregisterOnEnd = false, bool bAutoExecute = false);
    
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    void Execute(bool bResetExecution = true);
    void Tick(float DeltaTime);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    void End();
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    void EndBattleEvents();

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    void UpdateContextCandidates(TArray<UBattleActorComponent*> InCandidates);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    TArray<UBattleEvent*> GetBattleEvents() const;

    UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Element")
    FOnBattleElementDelegateBP OnBattleElementExecuted;
    UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Element")
    FOnBattleElementDelegateBP OnBattleElementEnded;

    FOnBattleElementDelegate OnBattleElementEnd;

    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleElementData> BattleElementData = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    bool bUnregisterOnEnd = false;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    bool bElementInExecution = false;

    float CachedDeltaTime = 0.f;

protected:

    void CreateBattleContext(TArray<UBattleActorComponent*> InTargets, TSubclassOf<UBattleContext> CustomContextClass);
    void DuplicateEvents();

    void EvaluateEvents(float DeltaTime);
    void EvaluateAsyncEvents(float DeltaTime);
    void StartCurrentEvent();
    void AdvanceEvent();

    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleActorComponent> Owner = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleContext> BattleContext = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Events")
    TArray<TObjectPtr<UBattleEvent>> Events;
    UPROPERTY(BlueprintReadOnly, Category = "Events")
    TArray<TObjectPtr<UBattleEvent>> ActiveAsyncEvents;
    UPROPERTY(BlueprintReadOnly, Category = "Events")
    TObjectPtr<UBattleEvent> CurrentEvent;
    UPROPERTY(BlueprintReadOnly, Category = "Events")
    int32 CurrentEventIndex = 0;
};

