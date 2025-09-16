// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "LevelSequencePlayer.h"
#include "SweetDreamsBattleManager.generated.h"

class USceneComponent;
class USweetDreamsWidget;
class ABattleCharacter;
class UBattleNumberWidget;
class UBattleElement;

UENUM(BlueprintType)
enum class ECameraView : uint8
{
	AllBattlers = 0,
	Allies = 1,
	Enemies = 2,
	Self = 3,
};

UENUM(BlueprintType)
enum class EBattlerType : uint8
{
	None = 0,
	Ally = 1,
	Enemy = 2,
};

UCLASS()
class SWEETDREAMSBATTLE_API ASweetDreamsBattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASweetDreamsBattleManager();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	FTimerHandle BattleTimer;

	UPROPERTY(BlueprintReadWrite, Category = "Battle Manager")
	bool bIsBattleActive = false;
	UPROPERTY(BlueprintReadWrite, Category = "Battle Manager")
	bool bIsVictorious = true;
	UPROPERTY(BlueprintReadWrite, Category = "Battle Manager")
	bool bBattlePaused = false;
	//
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleStart();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleEnd(bool bBattleVictorious);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnDamageApplied(AActor* DamageOwner, float Damage, bool bIsAllyDamage, bool bApplyCalculations);

	// COMPONENTS
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* BattleCamera;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	USceneComponent* BattleRoot;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	APlayerController* Player = nullptr;
	
	// UI
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FName BattleWidgetName = "Battle Widget";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<UBattleNumberWidget> DamageIndicatorClass;
	UPROPERTY(BlueprintReadWrite)
	USweetDreamsWidget* BattleWidget = nullptr;

	// BATLE
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Battlers", meta = (DisplayName = "Enemies References"))
	TArray<AActor*> Enemies;
	UPROPERTY(BlueprintReadWrite, Category = "Battlers", meta = (DisplayName = "Allies References"))
	TArray<AActor*> Allies;
	UPROPERTY(BlueprintReadWrite)
	TArray<float> AllyDamage;
	UPROPERTY(BlueprintReadWrite)
	TArray<float> EnemyDamage;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	bool bAutoMoveCamera = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera", meta = (DisplayName = "Battler Camera Blend Time", ClampMin = "0", Tooltip = "Time (in seconds) that the camera will blend between Battlers and between BattleManager camera to Battler camera."))
	float BattlerBlendTime = 1.0f;

public:	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 BattleIndex = 1;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static void StartBattleByIndex(const UObject* WorldContext, int32 Index);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsBattleManager* FindBattleByIndex(const UObject* WorldContext, int32 Index);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsBattleManager* FindActiveBattle(const UObject* WorldContext, UPARAM(DisplayName="Battle Index")int32& BattleId);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	virtual void SetBattlePaused(bool bPaused = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	virtual void StartBattle(float BlendTime = 2.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	virtual AActor* SpawnBattler(TSubclassOf<AActor> Battler, FTransform Transform, EBattlerType BattlerType = EBattlerType::Ally, USceneComponent* Root = nullptr, bool bAddToReferences = true);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void LoadBattlers();
	virtual void LoadBattlers_Implementation();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	virtual void EndBattle(float BlendTime = 2.0f);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager", meta = (ReturnDisplayName = "Battle Ended"))
	bool EvaluateEndBattle();
	virtual bool EvaluateEndBattle_Implementation();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	bool IsBattleOngoing() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	bool IsBattleVictorious() const;
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	virtual void ChangeCameraFocus(AActor* NewFocus, float BlendTime = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager", meta = (ToolTip = "When New View is Self and Actor Self Focus is valid, it will blend with the Camera Transform on Index 1 of the Actor instead."))
	virtual void ChangeCameraView(ECameraView NewView = ECameraView::AllBattlers, AActor* SelfFocus = nullptr, float BlendTime = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	virtual void AddDamageToBattle(AActor* DamageOwner, float Damage, bool bApplyCalculations);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	float GetAllAlliedDamage() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	float GetAllEnemyDamage() const;
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Battle Manager")
	TArray<AActor*>& GetBattlerGroup(EBattlerType BattlerType);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual TArray<AActor*> GetAllPossibleTargets(UBattleElement* Action, bool bUpdateCameraView = false);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	bool IsActorAlly(const AActor* Actor) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	bool IsActorEnemy(const AActor* Actor) const;
	// UI
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	TSubclassOf<UBattleNumberWidget> GetDamageIndicatorClass() const;
};
