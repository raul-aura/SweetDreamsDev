
#include "Components/ProjectileComponent.h"

UProjectileComponent::UProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectileComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProjectileTravel(DeltaTime);
}

void UProjectileComponent::InitializeProjectile(const FVector& InStartLocation, const FVector& InEndLocation, float InSpeed, bool bInDestroyOnEnd)
{
	if (GetOwner())
	{
		TimeElapsed = 0.f;
		TimeElapsedNormalized = 0.f;

		GetOwner()->SetActorLocation(InStartLocation);
		EndLocation = InEndLocation;
		Speed = InSpeed;
		bDestroyOnEnd = bInDestroyOnEnd;

		Direction = (EndLocation - GetOwner()->GetActorLocation()).GetSafeNormal();

		const float Distance = FVector::Dist(GetOwner()->GetActorLocation(), EndLocation);
		TotalTravelTime = Distance / Speed;

		bInitialized = true;
	}
}

void UProjectileComponent::ProjectileTravel(float DeltaTime)
{
	if (!bInitialized || !GetOwner()) return;

	TimeElapsed += DeltaTime;
	TimeElapsedNormalized = FMath::Clamp(TimeElapsed / TotalTravelTime, 0.f, 1.f);

	const FVector CurrentLocation = GetOwner()->GetActorLocation();
	const float Step = Speed * DeltaTime;

	if (TimeElapsedNormalized >= 1.f)
	{
		GetOwner()->SetActorLocation(EndLocation);

		OnReachedDestination.Broadcast();

		bInitialized = false;

		if (bDestroyOnEnd)
		{
			GetOwner()->Destroy();
		}

		return;
	}

	FHitResult Hit;
	GetOwner()->AddActorWorldOffset(Direction * Step, true, &Hit);
	OnProjectileTravel.Broadcast();

	if (Hit.IsValidBlockingHit())
	{
		OnProjectileHit.Broadcast(Hit);
	}
}


