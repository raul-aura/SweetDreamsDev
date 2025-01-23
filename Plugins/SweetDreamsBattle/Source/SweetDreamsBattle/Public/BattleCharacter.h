// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsCharacter.h"
#include "BattlerDataComponent.h"
#include "BattleAction.h"
#include "BattleCharacter.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API ABattleCharacter : public  ASweetDreamsCharacter
{
	GENERATED_BODY()

public:
	ABattleCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	// COMPONENTS
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	class UBoxComponent* BattleWorldArea;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	class UWidgetComponent* ParameterIndicator;
	// BATTLE
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG", meta = (InlineEditConditionToggle))
	bool bOverrideBattleIndicator = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG", meta = (EditCondition = "bOverrideBattleIndicator"))
	TSubclassOf<class UBattleNumberWidget> DamageIndicatorClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG")
	TSubclassOf<class UBattlerParameterWidget> ParameterIndicatorClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG", meta = (DisplayName = "Indicator Forward Offset"))
	float IndicatorOffset = 50.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG")
	float IndicatorPadding = 15.0f;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacked = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking = false;
	UPROPERTY(BlueprintReadWrite)
	bool bAttackOnCooldown = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG")
	float AttackCooldown = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG")
	bool bAttackStopsMovement = true;

public:	
	// DATA
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual UBattlerDataComponent* GetBattlerParameters() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	void OnAttack();
	void OnAttack_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	void OnDamageReceived(float Damage);
	void OnDamageReceived_Implementation(float Damage);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	float OnCharacterMitigateDamage(float Damage);
	float OnCharacterMitigateDamage_Implementation(float Damage);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	void IndicateDamage(float Value, bool bIsHealInstead = false);
	void IndicateDamage_Implementation(float Value, bool bIsHealInstead = false);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	void RemoveDamageIndicator(UWidgetComponent* Component);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	void IndicateParameter(UBattlerDataComponent* BattlerParameters);
	void IndicateParameter_Implementation(UBattlerDataComponent* BattlerParameters);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	void OnKilled(int32 CurrentLives);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	void OnRevived();
};
