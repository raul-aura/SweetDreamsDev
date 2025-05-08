// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattlerDataComponent.generated.h"

class USweetDreamsLevel;

UCLASS(ClassGroup = ("SweetDreams"), Blueprintable, meta = (BlueprintSpawnableComponent))
class SWEETDREAMSBATTLE_API UBattlerDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBattlerDataComponent();

protected:
	// PARAMS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battler")
	FText BattlerName = FText::FromString(TEXT("Battler"));
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battler")
	TArray<UTexture2D*> Portraits;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item RPG")
	TSubclassOf<USweetDreamsLevel> LevelClass;
	UPROPERTY(BlueprintReadWrite, Category = "Item RPG")
	USweetDreamsLevel* LevelObject = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0", ClampMax = "999"))
	int32 BaseActionsPerTurn = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0", ClampMax = "999"))
	int32 AdditionalActions = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0", ClampMax = "999"))
	int32 AdditionalLives = 0;
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Params")
	bool bIsAbleToAct = true;
	UPROPERTY(BlueprintReadOnly, Category = "Params")
	int32 CurrentLives = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Params")
	bool bIsDead = false;
	UPROPERTY(BlueprintReadOnly, Category = "Params", meta = (ClampMin = "0"))
	float ReviveDelay = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Params")
	bool bIsInBattle = false;
	// ELEMENTS

	// MULTIPLIERS


public:	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Battler Data")
	static UBattlerDataComponent* GetBattlerDataComponent(const AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual FText GetCharacterName() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void SetCharacterName(FText NewName);
	// LEVEL
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	USweetDreamsLevel* GetLevelObject() const { return LevelObject; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	int32 GetLevelNumber() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	void SetLevelNumber(int32 NewLevel = 1);
	// PARAMS

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	void Kill(AActor* KillInstigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	void Revive(float HealthRestore = 100.f, float ManaRestore = 100.f); 
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual bool IsDead() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual bool IsInBattle() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battler Data")
	virtual void SetInBattle(ASweetDreamsBattleManager* BattleReference, bool bNewIsInBattle = true);
	// STATES

	// ACTIONS

	// MULTIPLIERS
};
