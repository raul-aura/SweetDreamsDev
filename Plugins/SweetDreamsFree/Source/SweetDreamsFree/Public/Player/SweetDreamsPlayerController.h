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

	// Gets the Character casted to SweetDreamsCharacter class.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	ASweetDreamsCharacter* GetDreamCharacter() const;

	// Gets DreamCharacter and calls the internal MoveForward() function, if valid.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void MoveForward(float Value);
	// Gets DreamCharacter and calls the internal MoveRight() function, if valid.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void MoveRight(float Value);
	// Gets DreamCharacter and calls the internal Run() function, if valid.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void Run();
	// Gets DreamCharacter and calls the internal StopRunning() function, if valid.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void StopRunning();
	// Gets DreamCharacter and calls the internal CameraVertical() function, if valid.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void CameraVertical(float Value, float Sensitivity = 1.0f);
	// Gets DreamCharacter and calls the internal CameraHorizontal() function, if valid.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void CameraHorizontal(float Value, float Sensitivity = 1.0f);
	// Disables the input of this controller for a defined duration with a timer, then re-enables the input.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player", meta = (DisplayName = "Toggle Input (Timer)"))
	void ToggleInputTimer(float Duration = 1.f);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FTimerHandle InputTimer;
};
