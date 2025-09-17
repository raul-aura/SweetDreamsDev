// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/DataTable.h"
#include "MulticameraComponent.generated.h"

USTRUCT(BlueprintType)
struct SWEETDREAMS_API FCameraView : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Multicamera", meta = (DisplayName = "Camera Location"))
	FVector Location = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Multicamera", meta = (DisplayName = "Camera Rotation"))
	FRotator Rotation = FRotator::ZeroRotator;

	FCameraView()
		: Location(FVector(0)),
		Rotation(FRotator(0))
	{}

	FCameraView(const FVector& InLocation, const FRotator& InRotation)
		: Location(InLocation),
		Rotation(InRotation)
	{}
};

UCLASS(ClassGroup = ("SweetDreams"), meta = (BlueprintSpawnableComponent, ToolTip = "This component enables the management of multiple cameras on the same actor. Similar to a View Target Blend, but with internal cameras inside a single actor.\n\nUse the function SetNewCameraView() to transfer the settings of a secondary camera to the primary one."))
class SWEETDREAMS_API UMulticameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMulticameraComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Multicamera", CallInEditor)
	void UpdateCameraView();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Multicamera")
	void AddCameraView(FVector Location, FRotator Rotation, int32 Index = -1);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Multicamera", meta = (ToolTip = "Transfer the properties of the camera to match to the primary camera."))
	void SetNewCameraView(int32 CameraToMatch = 0, float BlendTime = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Multicamera", meta = (ToolTip = "Defines a new camera to be the current one. Ideal when you have more than one camera."))
	void SetActiveCamera(UPARAM(ref) UCameraComponent*& NewCamera);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Multicamera")
	TArray<FCameraView> GetAllPossibleViews() const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Multicamera")
	int32 CurrentView = 0;

protected:
	virtual void BeginPlay() override;
	void FindCamera();
	void CameraBlend();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multicamera")
	TArray<FCameraView> CameraViews;

	// COMPONENTS
	UPROPERTY(BlueprintReadWrite, Category = "Multicamera")
	UCameraComponent* ActiveCamera = nullptr;

	// BLEND
	UPROPERTY(BlueprintReadOnly, Category = "Multicamera")
	FTimerHandle BlendHandle;
	UPROPERTY(BlueprintReadOnly, Category = "Multicamera")
	float BlendElapsedTime = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Multicamera")
	float BlendTotalTime = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multicamera")
	UCurveFloat* AlphaCurve = nullptr;

	// CAMERA PARAMS
	FVector StartLocation = FVector::ZeroVector;
	FVector EndLocation = FVector::ZeroVector;
	FRotator StartRotation = FRotator::ZeroRotator;
	FRotator EndRotation = FRotator::ZeroRotator;
};
