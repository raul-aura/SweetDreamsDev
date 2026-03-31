
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/BattleDataTypes.h"
#include "BattleActorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleActorDelegate, UBattleActorComponent*, BattleActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageDealt, UBattleActorComponent*, Instigator, const FDamageHealResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(
	FOnDamageReceived,
	UBattleActorComponent*, Target,
	UBattleActorComponent*, Instigator,
	FGameplayTagContainer, DamageTags,
	float, Damage,
	float, PreviousHealth,
	float, NewHealth
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealDealt, UBattleActorComponent*, Instigator, const FDamageHealResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(
	FOnHealReceived, 
	UBattleActorComponent*, Target, 
	UBattleActorComponent*, Instigator,
	FGameplayTagContainer, HealTags,
	float, Heal,
	float, PreviousHealth,
	float, NewHealth
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnElementExecuted, UBattleActorComponent*, BattleActor, UBattleElement*, Element);

class UBattleElement;

UCLASS( ClassGroup=("SweetDreams"), Blueprintable, meta = (BlueprintSpawnableComponent))
class SWEETDREAMSBATTLE_API UBattleActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBattleActorComponent();

	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Battle|Battle Actor")
	static UBattleActorComponent* GetBattleActorComponent(const AActor* Actor);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool ExecuteBattleElement(const UBattleElementData* ElementData, UBattleElement*& OutElement);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool RegisterBattleElement(UBattleElement* BattleElement);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void UnregisterBattleElement(UBattleElement* BattleElement);
	
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	FDamageHealTargetResult ReceiveDamage(UBattleActorComponent* Instigator, FGameplayTagContainer DamageTags, float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	FDamageHealTargetResult ReceiveHeal(UBattleActorComponent* Instigator, FGameplayTagContainer HealTags, float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void SetIsAlive(bool bInIsAlive);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void SetInCombat(bool bInIsInCombat);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool IsDead() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool IsInCombat() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	ETeamType GetTeam() const;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	UBattleElement* GetBattleElementByIndex(int32 Index = 0) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	UBattleElement* GetBattleElementByData(UBattleElementData* ElementData) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor", meta = (DeterminesOutputType = "EventClass"))
	UBattleEvent* GetBattleEventByClass(const UBattleElement* BattleElement, TSubclassOf<UBattleEvent> EventClass) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	TArray<UBattleElement*> GetBattleElements() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	TArray<UBattleElement*> GetBattleElementsInExecution() const;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void InitializeParameter(UPARAM(ref) FBattleParameter& Parameter);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	FBattleParameterModifier AddModifierToParameter(FGameplayTag ParameterTag, EParameterModifierType ModifierType, float ModifierValue);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void RemoveModifierFromParameter(FGameplayTag ParameterTag, FBattleParameterModifier Modifier);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void IncreaseParameterResource(FGameplayTag ParameterTag, float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void DecreaseParameterResource(FGameplayTag ParameterTag, float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool GetParameter(FGameplayTag ParameterTag, FBattleParameter& OutParam) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool GetHealthParameter(FBattleParameter& OutParam) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool GetParameterValues(FGameplayTag ParameterTag, float& OutParameterValue, float& OutResourceValue) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool GetHealthParameterValues(float& OutParameterValue, float& OutResourceValue) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	float GetSimpleParameter(FGameplayTag ParameterTag, float Percentage = 1.f) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Battle Actor")
	bool CanExecuteBattleElement(UBattleElement* Element) const;
	bool CanExecuteBattleElement_Implementation(UBattleElement* Element) const;
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle|Battle Actor")
	void OnBattleElementExecuted_Event(UBattleElement* Element);
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Battle Actor")
	float ModifyDamageReceived(float Damage);
	float ModifyDamageReceived_Implementation(float Damage);
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Battle Actor")
	float ModifyHealingReceived(float Healing);
	float ModifyHealingReceived_Implementation(float Healing);

	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Actor")
	FOnBattleActorDelegate OnEnterCombat;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Actor")
	FOnBattleActorDelegate OnExitCombat;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Actor")
	FOnBattleActorDelegate OnKilled;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Actor")
	FOnBattleActorDelegate OnRessurected;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Actor")
	FOnDamageDealt OnDamageDealt;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Actor")
	FOnDamageReceived OnDamageReceived;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Actor")
	FOnHealDealt OnHealingDealt;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Actor")
	FOnHealReceived OnHealingReceived;
	UPROPERTY(BlueprintAssignable, Category = "Sweet Dreams|Battle|Battle Actor")
	FOnElementExecuted OnBattleElementExecuted;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Actor", meta = (ExposeOnSpawn = true))
	ETeamType Team = ETeamType::None;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parameters", meta = (ExposeOnSpawn = true))
	FGameplayTag HealthParameterTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters", meta = (ExposeOnSpawn = true))
	TMap<FGameplayTag, FBattleParameter> Parameters;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters", meta = (ExposeOnSpawn = true))
	TMap<FGameplayTag, float> SimpleParameters;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Elements")
	TArray<TObjectPtr<UBattleElement>> BattleElements;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Battle Actor")
	bool bIsAlive = true;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Actor")
	bool bIsInCombat = false;
};
