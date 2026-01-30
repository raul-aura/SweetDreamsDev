#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/BattleDataTypes.h"
#include "BattleElement.generated.h"

DECLARE_DELEGATE_OneParam(FOnBattleElementEnd, UBattleElement* /* BattleElement */);

class UBattleEvent;
class UBattleElementData;
class UBattleActorComponent;

UCLASS(BlueprintType)
class SWEETDREAMSBATTLE_API UBattleElement : public UObject
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Element", meta = (WorldContext = "BattleComponent", CallableWithoutWorldContext, DeterminesOutputType = "CustomClass"))
    static UBattleElement* CreateBattleElement(UBattleActorComponent* BattleComponent, UBattleElementData* Data, TSubclassOf<UBattleElement> CustomClass);
    
    void Start(/*UBattleContext* Context*/);
    void Tick(float DeltaTime);
    void End();

    FOnBattleElementEnd OnBattleElementEnd;

protected:

    void DuplicateEvents();


    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleElementData> BattleElementData = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "Data")
    TObjectPtr<UBattleActorComponent> Owner = nullptr;

    // TO DO: move these 3 events to a struct 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Events")
    TArray<FBattleElementEventPhase> Phases;
};

