
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/BattleDataTypes.h"
#include "BattleActorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleActorDelegate, UBattleActorComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBattleActorEffect, UBattleActorComponent*, Instigator, UBattleActorComponent*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBattleActorValueEffect, UBattleActorComponent*, Instigator, UBattleActorComponent*, Target, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBattleActorParameterChange, FBattleParamater, Parameter, FBattleParameterModifier, Modifier);

UCLASS( ClassGroup=("SweetDreams"), meta = (BlueprintSpawnableComponent))
class SWEETDREAMSBATTLE_API UBattleActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBattleActorComponent();

	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Battle|Battle Actor")
	static UBattleActorComponent* GetBattleActorComponent(const AActor* Actor);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void Damage(AActor* Target, float Amount);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void ReceiveDamage(AActor* Instigator, float Amount);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void Heal(AActor* Target, float Amount);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void ReceiveHeal(AActor* Instigator, float Amount);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void Kill(AActor* Instigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void Kill_Target(AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void Revive(AActor* Instigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void Revive_Target(AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void SetInCombat(bool bInIsInCombat);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool IsInCombat() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void AddModifierToParameter(UPARAM(ref) FBattleParamater& Parameter, EParameterModifierType ModifierType, float ModifierValue);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void RemoveModifierFromParameter(UPARAM(ref) FBattleParamater& Parameter, FBattleParameterModifier Modifier);

	UPROPERTY(BlueprintAssignable)
	FOnBattleActorDelegate OnEnterCombatDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorDelegate OnExitCombatDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorValueEffect OnDamageDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorValueEffect OnReceiveDamageDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorValueEffect OnHealDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorValueEffect OnReceiveHealDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorEffect OnKillDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorEffect OnReceiveKillDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorEffect OnReviveDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorEffect OnReceiveReviveDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorParameterChange OnParameterChangeDelegate;

protected:
	virtual void BeginPlay() override;

	TArray<UObject*> GetBattleDependents() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	FBattleParamater Health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	FBattleParamater Strength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	FBattleParamater Resistence;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	TMap<FName, FBattleParamater> CustomParameters;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	TMap<FName, float> CustomSimpleParameters;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data", meta = (ExposeOnSpawn=true))
	ETeamType Team = ETeamType::None;
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	float DamageDealt = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	float HealingDealt = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bIsAlive = true;
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bIsInCombat = false;

};
