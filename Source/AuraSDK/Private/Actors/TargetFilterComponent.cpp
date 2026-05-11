
#include "Actors/TargetFilterComponent.h"

constexpr float Half = 0.5f;

UTargetFilterComponent::UTargetFilterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTargetFilterComponent::UpdateTargetsReference(TArray<AActor*> InTargets)
{
	Targets = InTargets;
}

AActor* UTargetFilterComponent::GetTargetClosestToScreenCenter(const APlayerController* PlayerController) const
{
    if (!IsValid(PlayerController)) return nullptr;

    if (Targets.IsEmpty())
    {
        CurrentTarget = nullptr;
        return nullptr;
    }

    int32 SizeX = 0;
    int32 SizeY = 0;
    PlayerController->GetViewportSize(SizeX, SizeY);

    const FVector2D ScreenCenter(SizeX * Half, SizeY * Half);
    const float MaxRadius = FMath::Min(SizeX, SizeY) * ScreenRadiusPercent;
    const float MaxRadiusSq = MaxRadius * MaxRadius;

    float BestScore = TNumericLimits<float>::Max();
    const AActor* Previous = CurrentTarget;
    AActor* BestTarget = nullptr;

    for (AActor* Actor : Targets)
    {
        if (IsValid(Actor))
        {
            // TO DO: pass a bool that will act as an additional rule to filter
            if (!IsTargetBehindCamera(PlayerController, Actor) /* && RULE */)
            {
                FVector2D ScreenPos;
                if (PlayerController->ProjectWorldLocationToScreen(Actor->GetActorLocation(), ScreenPos))
                {
                    const float ScreenDistSq = FVector2D::DistSquared(ScreenCenter, ScreenPos);

                    if (bEnableDebug)
                    {
                        DebugTargets(Actor, 
                            PlayerController->PlayerCameraManager->GetActorForwardVector(), 
                            PlayerController->PlayerCameraManager->GetCameraLocation(), 
                            MaxRadiusSq, ScreenDistSq);
                    }

                    if (ScreenDistSq <= MaxRadiusSq && ScreenDistSq < BestScore)
                    {
                        BestScore = ScreenDistSq;
                        BestTarget = Actor;
                    }
                }
            }
        }
    }

    if (BestTarget != CurrentTarget)
    {
        CurrentTarget = BestTarget;
        OnTargetUpdated.Broadcast(CurrentTarget, Previous);
    }

    return BestTarget;
}

bool UTargetFilterComponent::IsTargetBehindCamera(const APlayerController* PlayerController, const AActor* Target) const
{
    const FVector Forward = PlayerController->PlayerCameraManager->GetActorForwardVector();
    const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
    const FVector Dir = (Target->GetActorLocation() - CameraLocation).GetSafeNormal();

    return FVector::DotProduct(Forward, Dir) < 0.f;
}

void UTargetFilterComponent::DebugTargets(const AActor* Target, const FVector& Forward, const FVector& CameraLocation, const float& Radius, const float& ScreenDist) const
{
    DrawDebugCircle(
        GetWorld(),
        CameraLocation,
        FMath::Sqrt(Radius),
        32,
        FColor::Green,
        false,
        -1.f,
        0,
        2.f,
        Forward.Rotation().Quaternion().GetAxisZ(),
        Forward.Rotation().Quaternion().GetAxisY(),
        false
    );

    const bool bInside = ScreenDist <= Radius;

    DrawDebugLine(
        GetWorld(), CameraLocation, Target->GetActorLocation(), bInside ? FColor::Green : FColor::Red, false,
        -1.f,
        0,
        2.f
    );
}
