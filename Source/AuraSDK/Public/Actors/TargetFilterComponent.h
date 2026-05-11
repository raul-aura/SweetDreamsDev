
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetFilterComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetFilterDelegate, const AActor*, NewTarget, const AActor*, PreviousTarget);

UCLASS(ClassGroup = ("AuraSDK"), meta = (BlueprintSpawnableComponent))
class AURASDK_API UTargetFilterComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UTargetFilterComponent();

	UFUNCTION(BlueprintCallable, Category = "AuraSDK|Target Filter Component")
	void UpdateTargetsReference(TArray<AActor*> InTargets);

	UFUNCTION(BlueprintCallable, Category = "AuraSDK|Target Filter Component")
	AActor* GetTargetClosestToScreenCenter(const APlayerController* PlayerController) const;

	UPROPERTY(BlueprintAssignable, Category = "AuraSDK|Target Filter Component")
	FOnTargetFilterDelegate OnTargetUpdated;

protected:

	bool IsTargetBehindCamera(const APlayerController* PlayerController, const AActor* Target) const;

	void DebugTargets(const AActor* Target, const FVector& Forward, const FVector& CameraLocation, const float& Radius, const float& ScreenDist) const;

	UPROPERTY(EditDefaultsOnly, Category = "Target Filter Component")
	float ScreenRadiusPercent = 0.15f;
	UPROPERTY(EditDefaultsOnly, Category = "Target Filter Component")
	float MaxCenterAngle = 45.f;
	UPROPERTY(EditDefaultsOnly, Category = "Target Filter Component")
	bool bEnableDebug = false;

	TArray<TObjectPtr<AActor>> Targets;

	mutable TObjectPtr<AActor> CurrentTarget;
};

