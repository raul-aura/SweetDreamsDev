

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileHit, FHitResult, HitResult);

UCLASS( ClassGroup=("AuraSDK"), meta=(BlueprintSpawnableComponent) )
class AURASDK_API UProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UProjectileComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Projectile Component")
	void InitializeProjectile(const FVector& InStartLocation, const FVector& InEndLocation, float InSpeed, bool bInDestroyOnEnd);
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Projectile Component")
	void ProjectileTravel(float DeltaTime);

	UPROPERTY(BlueprintAssignable, Category = "AuraSDK | Projectile Component")
	FOnProjectileDelegate OnProjectileTravel;
	UPROPERTY(BlueprintAssignable, Category = "AuraSDK | Projectile Component")
	FOnProjectileDelegate OnReachedDestination;
	UPROPERTY(BlueprintAssignable, Category = "AuraSDK | Projectile Component")
	FOnProjectileHit OnProjectileHit;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile Component")
	FVector EndLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Projectile Component")
	FVector Direction = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Projectile Component")
	float Speed = 100.f;
	UPROPERTY(BlueprintReadOnly, Category = "Projectile Component")
	bool bDestroyOnEnd = true;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile Component")
	float TimeElapsed = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Projectile Component")
	float TimeElapsedNormalized = 0.f;

private:

	bool bInitialized = false;

	float TotalTravelTime = 0.f;
};
