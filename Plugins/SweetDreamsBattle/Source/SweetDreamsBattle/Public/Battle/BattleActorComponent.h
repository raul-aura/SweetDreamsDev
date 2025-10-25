

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/BattleDataTypes.h"
#include "BattleActorComponent.generated.h"


UCLASS( ClassGroup=("SweetDreams"), meta = (BlueprintSpawnableComponent))
class SWEETDREAMSBATTLE_API UBattleActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBattleActorComponent();

	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|Battle|Battle Actor")
	static UBattleActorComponent* GetBattleActorComponent(const AActor* Actor);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Damage(AActor* Target, float Amount);
	void ReceiveDamage(AActor* Instigator, float Amount);

	void Heal(AActor* Target, float Amount);
	void ReceiveHeal(AActor* Instigator, float Amount);

	void Kill();
	void Kill(AActor* Target);
	void Ressurect();
	void Ressurect(AActor* Target);

	void SetInCombat(bool bInIsInCombat);

	bool IsAlive() const;
	bool IsInCombat() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	FBattleParamater Health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	FBattleParamater Strength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	FBattleParamater Resistence;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	TMap<FName, FBattleParamater> CustomParameters;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data")
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
