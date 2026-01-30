
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/BattleDataTypes.h"
#include "BattleActorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleActorDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetEffect, AActor*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveEffect, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReceiveValueEffect, AActor*, Instigator, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetValueEffect, AActor*, Target, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBattleActorParameterChange, FBattleParamater, Parameter, FBattleParameterModifier, Modifier);

class UBattleElement;

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
	bool RegisterBattleElement(UBattleElement* BattleElement);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void UnregisterBattleElement(UBattleElement* BattleElement);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void Damage(AActor* Target, float Amount);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void ReceiveDamage(AActor* Instigator, float Amount);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void Heal(AActor* Target, float Amount);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void ReceiveHeal(AActor* Instigator, float Amount);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void ReceiveKill(AActor* Instigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void KillTarget(AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void ReceiveRevive(AActor* Instigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void ReviveTarget(AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void SetInCombat(bool bInIsInCombat);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	bool IsInCombat() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	ETeamType GetTeam() const;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void AddModifierToParameter(UPARAM(ref) FBattleParamater& Parameter, EParameterModifierType ModifierType, float ModifierValue);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void RemoveModifierFromParameter(UPARAM(ref) FBattleParamater& Parameter, FBattleParameterModifier Modifier);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	FBattleParamater GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	FBattleParamater GetStrength() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	FBattleParamater GetResistence() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	FBattleParamater GetCustomParameter(FName Parameter) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	float GetCustomSimpleParameter(FName Parameter, float Percentage = 1.f) const;

	UPROPERTY(BlueprintAssignable)
	FOnBattleActorDelegate OnEnterCombat;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorDelegate OnExitCombat;
	UPROPERTY(BlueprintAssignable)
	FOnTargetValueEffect OnDamage;
	UPROPERTY(BlueprintAssignable)
	FOnReceiveValueEffect OnReceiveDamage;
	UPROPERTY(BlueprintAssignable)
	FOnTargetValueEffect OnHeal;
	UPROPERTY(BlueprintAssignable)
	FOnReceiveValueEffect OnReceiveHeal;
	UPROPERTY(BlueprintAssignable)
	FOnTargetEffect OnKill;
	UPROPERTY(BlueprintAssignable)
	FOnReceiveEffect OnReceiveKill;
	UPROPERTY(BlueprintAssignable)
	FOnTargetEffect OnRevive;
	UPROPERTY(BlueprintAssignable)
	FOnReceiveEffect OnReceiveRevive;
	UPROPERTY(BlueprintAssignable)
	FOnBattleActorParameterChange OnParameterChange;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Battle|Battle Actor")
	void InitiateCombat(AActor* OtherActor, bool bCheckForHostility = true);

	TArray<UObject*> GetBattleDependents() const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Actor", meta = (ExposeOnSpawn = true))
	ETeamType Team = ETeamType::None;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Actor", meta = (ExposeOnSpawn = true))
	bool bStartCombatOnDamage = true;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Actor")
	float DamageDealt = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Actor")
	float HealingDealt = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Actor")
	bool bIsAlive = true;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Actor")
	bool bIsInCombat = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters", meta = (ExposeOnSpawn = true))
	FBattleParamater Health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters", meta = (ExposeOnSpawn = true))
	FBattleParamater Strength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters", meta = (ExposeOnSpawn = true))
	FBattleParamater Resistence;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters", meta = (ExposeOnSpawn = true))
	TMap<FName, FBattleParamater> CustomParameters;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters", meta = (ExposeOnSpawn = true))
	TMap<FName, float> CustomSimpleParameters;

	UPROPERTY(BlueprintReadOnly, Category = "Battle Elements")
	TArray<TObjectPtr<UBattleElement>> BattleElements;

};
