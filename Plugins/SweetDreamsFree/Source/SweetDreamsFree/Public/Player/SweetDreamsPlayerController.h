// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SweetDreamsPlayerController.generated.h"

class ASweetDreamsCharacter;

UCLASS()
class SWEETDREAMSFREE_API ASweetDreamsPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	ASweetDreamsCharacter* GetDreamCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void MoveForward(float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void MoveRight(float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void Run();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void StopRunning();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void CameraVertical(float Value, float Sensitivity = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void CameraHorizontal(float Value, float Sensitivity = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player", meta = (DisplayName = "Toggle Input (Timer)"))
	void ToggleInputTimer(float Duration = 1.f);
};
