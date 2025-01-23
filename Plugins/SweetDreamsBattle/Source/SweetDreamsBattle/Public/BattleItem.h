// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "SweetDreamsItem.h"
#include "BattleElement.h"
#include "SweetDreamsLevel.h"
#include "BattleItem.generated.h"

UCLASS(BlueprintType)
class SWEETDREAMSBATTLE_API USweetDreamsItemRPG : public USweetDreamsItem
{
	GENERATED_BODY()

public:
	USweetDreamsItemRPG() { ; }
	void OnCreateItem_Implementation() override;
	void OnEquipItem_Implementation() override;
	void OnUnequipItem_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Item")
	virtual void ApplyParams();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Item")
	virtual void RemoveParams();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Item")
	virtual void UpgradeItem();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Item")
	virtual void UpdateParams();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Item")
	virtual float UpdateParameterByLevel(float& Parameter, float BaseParameter = 10.f, float AdditionalParameter = 5.f, float QuadraticFactor = 0.2f);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item RPG")
	TSubclassOf<USweetDreamsLevel> LevelClass = USweetDreamsLevel::StaticClass();
	UPROPERTY(BlueprintReadWrite, Category = "Item RPG")
	USweetDreamsLevel* LevelObject = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item RPG")
	bool bApplyParamsOnAdd = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item RPG")
	bool bRemoveParamsOnRemove = true;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	float BaseHealth = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params", meta = (Units = "%"))
	float BaseHealthPercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	float BaseMana = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params", meta = (Units = "%"))
	float BaseManaPercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	float BaseForce = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params", meta = (Units = "%"))
	float BaseForcePercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	float BaseResistence = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params", meta = (Units = "%"))
	float BaseResistencePercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	int32 BaseSpeed = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params", meta = (Units = "%"))
	float BaseSpeedPercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	int32 BaseActions = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Params")
	int32 BaseLives = 0;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params")
	float AdditionalHealth = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params", meta = (Units = "%"))
	float AdditionalHealthPercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params")
	float AdditionalMana = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params", meta = (Units = "%"))
	float AdditionalManaPercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params")
	float AdditionalForce = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params", meta = (Units = "%"))
	float AdditionalForcePercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params")
	float AdditionalResistence = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params", meta = (Units = "%"))
	float AdditionalResistencePercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params")
	int32 AdditionalSpeed = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params", meta = (Units = "%"))
	float AdditionalSpeedPercentage = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params")
	int32 AdditionalActions = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Additional Params")
	int32 AdditionalLives = 0;
	//
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Health = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float HealthPercentage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Mana = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float ManaPercentage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Force = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float ForcePercentage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float Resistence = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float ResistencePercentage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	int32 Speed = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float SpeedPercentage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	int32 Actions = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	int32 Lives = 0;
	// ACTIONS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Extras", meta = (DisplayName = "Actions"))
	TArray<TSubclassOf<UBattleAction>> ActionClasses;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Extras", meta = (DisplayName = "States"))
	TArray<TSubclassOf<UBattleState>> StateClasses;
	// MULTIPLIERS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float ForceMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float ResistenceMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float SpeedMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float DamageDealtMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float DamageReceivedMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float HealMultiplier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float ManaRestoreMultiplier = 0;
};

UCLASS(BlueprintType)
class SWEETDREAMSBATTLE_API USweetDreamsWeapon : public USweetDreamsItemRPG
{
	GENERATED_BODY()

public:

};

UCLASS(BlueprintType)
class SWEETDREAMSBATTLE_API USweetDreamsArmor : public USweetDreamsItemRPG
{
	GENERATED_BODY()

public:
};
