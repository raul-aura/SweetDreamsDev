// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MulticameraComponent.h"
#include "SweetDreamsController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SweetDreamsCharacter.generated.h"

UCLASS()
class SWEETDREAMS_API ASweetDreamsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASweetDreamsCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UMulticameraComponent* MulticameraComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	float WalkSpeed = 600.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	float RunSpeed = 900.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	float MaxRunTime = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	bool bIsRunning = false;
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	bool bCanRun = false;
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	float CurrentRunTime = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	FTimerHandle RunTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	bool bCanMove = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	bool bCanMoveCamera = true;

public:	
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	ASweetDreamsController* GetDreamController() const;
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void MoveForward(float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void MoveRight(float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void Run();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void StopRunning();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void CameraVertical(float Value, float Sensitivity = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void CameraHorizontal(float Value, float Sensitivity = 1.0f);
};
