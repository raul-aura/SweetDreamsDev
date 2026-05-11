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

UCLASS(BlueprintType, Blueprintable)
class SWEETDREAMSBATTLE_API UBattleElement : public UObject
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element", meta = (DeterminesOutputType = "CustomClass", AdvancedDisplay = 2, AutoCreateRefTerm = "Targets"))
    static UBattleElement* CreateBattleElement(UBattleActorComponent* InOwner, UBattleElementData* Data, TSubclassOf<UBattleElement> CustomClass);
    bool InitializeBattleElement(UBattleActorComponent* InOwner, UBattleElementData* Data);

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    void Execute(bool bResetExecution = true);
    void Tick(float DeltaTime);
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    void End();
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    void EndBattleEvents();

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Context")
    TArray<UBattleActorComponent*> GetSelectedTargets(const FSelectedTargetsSettings& Settings) const;
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    UBattleActorComponent* GetOwner() const;
    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element")
    TArray<UBattleEvent*> GetBattleEvents() const;

    FOnBattleElementDelegate OnBattleElementEnd;
    UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Element")
    FOnBattleElementDelegateBP OnBattleElementExecuted;
    UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Element")
    FOnBattleElementDelegateBP OnBattleElementEnded;

    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleElementData> BattleElementData = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    bool bUnregisterOnEnd = false;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    bool bElementInExecution = false;

    float CachedDeltaTime = 0.f;

protected:

    void DuplicateEvents();

    void EvaluateEvents(float DeltaTime);
    void StartCurrentEvent();
    void AdvanceEvent();
    void HandleEventsComplete();

    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleActorComponent> Owner = nullptr;
    UPROPERTY(BlueprintReadWrite, Category = "Battle Context")
    TArray<TObjectPtr<UBattleActorComponent>> CandidateBattleActors;

    UPROPERTY(BlueprintReadOnly, Category = "Data")
    bool bAllEventsCompleted = false;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    bool bAutoEnd = false;

    UPROPERTY(BlueprintReadOnly, Category = "Events")
    TArray<TObjectPtr<UBattleEvent>> Events;
    UPROPERTY(BlueprintReadOnly, Category = "Events")
    TObjectPtr<UBattleEvent> CurrentEvent;
    UPROPERTY(BlueprintReadOnly, Category = "Events")
    int32 CurrentEventIndex = 0;
};

