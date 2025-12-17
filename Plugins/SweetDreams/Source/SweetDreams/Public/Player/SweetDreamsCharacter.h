// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SweetDreamsCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SWEETDREAMS_API ASweetDreamsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASweetDreamsCharacter();

	// Adds movement input in the forward direction of this character.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void MoveForward(float Value);
	// Adds movement input in the right direction of this character.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void MoveRight(float Value);
	// Changes the MaxWalkSpeed of the character movement to the running value.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void Run();
	// Changes the MaxWalkSpeed of the character movement to the walk value.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void StopRunning();
	// Adds input to the controller control rotation, affecting pitch and adjusted by sensitivity.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void CameraVertical(float Value, float Sensitivity = 1.0f);
	// Adds input to the controller control rotation, affecting yaw and adjusted by sensitivity.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void CameraHorizontal(float Value, float Sensitivity = 1.0f);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	float WalkSpeed = 600.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	float RunSpeed = 900.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	float CrouchSpeed = 400.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	float MaxRunTime = 0.f;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Run, Category = "Core")
	bool bIsRunning = false;
	UFUNCTION(Category = "Core")
	void OnRep_Run();
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	float CurrentRunTime = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	FTimerHandle RunTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	bool bCanMove = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	bool bCanRun = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	bool bCanMoveCamera = true;

	UFUNCTION(Server, Reliable)
	void ServerRun(bool bRunning);
	void Run_Internal();
	void StopRunning_Internal();

};
