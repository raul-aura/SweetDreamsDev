// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/InteractComponent.h"
#include "Game/SweetDreamsInteractInterface.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bDrawDebugRanged)
	{
		FVector CharacterLocation = GetOwner()->GetActorLocation();
		DrawDebugSphere(GetWorld(), CharacterLocation, InteractRadius, 12, FColor::Green, false, -1.f, 0, 5.f);
	}
	if (bDrawDebugLine)
	{
		UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
		FVector Start = Camera->GetComponentLocation();
		FVector End = Start + (Camera->GetForwardVector() * InteractTraceDistance);
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 5.f);
	}
}

TArray<AActor*> UInteractComponent::FindInteractablesInRange()
{
	TArray<AActor*> PreviousActors = ActorsWithinRange;
	ActorsWithinRange.Reset();
	FVector Origin = GetOwner()->GetActorLocation();
	FCollisionShape Shape = MakeRangedShape();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(GetIgnoredActors());
	TArray<FOverlapResult> Overlaps;
	if (GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams::AllObjects, Shape, QueryParams))
	{
		for (const FOverlapResult& Result : Overlaps)
		{
			AActor* HitActor = Result.GetActor();
			if (!IsValid(HitActor)) continue;
			const bool bIsInteractable = HitActor->Implements<USweetDreamsInteractInterface>();
			if (!bLimitToInteractableInterface || bIsInteractable)
			{
				ActorsWithinRange.Add(HitActor);
				if (!PreviousActors.Contains(HitActor) && bIsInteractable)
				{
					ISweetDreamsInteractInterface::Execute_OnBeginTrace(HitActor, GetOwner());
				}
			}
		}
	}
	InvalidateActorsInRange(PreviousActors);
	return ActorsWithinRange;
}

AActor* UInteractComponent::FindInteractableTraced()
{
	FHitResult HitResult;
	UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * InteractTraceDistance);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(GetIgnoredActors());
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, QueryParams);
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (IsValid(HitActor))
		{
			const bool bIsInteractable = HitActor->Implements<USweetDreamsInteractInterface>();
			if (!bLimitToInteractableInterface || bIsInteractable)
			{
				if (ActorTraceHit != HitActor)
				{
					InvalidateActorTraced();
					ActorTraceHit = HitActor;
					if (bIsInteractable)
					{
						ISweetDreamsInteractInterface::Execute_OnBeginTrace(HitActor, GetOwner());
					}
				}
			}
			else
			{
				InvalidateActorTraced();
			}
		}
	}
	else
	{
		InvalidateActorTraced();
	}
	return ActorTraceHit;
}

bool UInteractComponent::InteractTraced(AActor*& ActorTraced)
{
	if (!IsValid(ActorTraceHit)) return false;
	ActorTraced = ActorTraceHit;
	return InteractActor(ActorTraced);
}

bool UInteractComponent::InteractRanged(int32 Index, TArray<AActor*>& ActorsFound)
{
	if (ActorsWithinRange.Num() == 0 || !ActorsWithinRange.IsValidIndex(Index)) return false;
	return InteractActor(ActorsWithinRange[Index]);
}

bool UInteractComponent::InteractActor(AActor* Interactable)
{
	if (IsValid(Interactable) && Interactable->Implements<USweetDreamsInteractInterface>())
	{
		ISweetDreamsInteractInterface::Execute_OnInteract(Interactable, GetOwner());
		return true;
	}
	return false;
}

void UInteractComponent::InvalidateActorTraced()
{
	if (IsValid(ActorTraceHit) && ActorTraceHit->Implements<USweetDreamsInteractInterface>())
	{
		ISweetDreamsInteractInterface::Execute_OnEndTrace(ActorTraceHit, GetOwner());
	}
	ActorTraceHit = nullptr;
}

void UInteractComponent::InvalidateActorsInRange(const TArray<AActor*>& PreviousActors)
{
	for (AActor* PrevActor : PreviousActors)
	{
		if (!ActorsWithinRange.Contains(PrevActor) && IsValid(PrevActor) && PrevActor->Implements<USweetDreamsInteractInterface>())
		{
			ISweetDreamsInteractInterface::Execute_OnEndTrace(PrevActor, GetOwner());
		}
	}
}

FCollisionShape UInteractComponent::MakeRangedShape() const
{
	switch (RangedShape)
	{
	case ERangedTraceShape::Sphere: return FCollisionShape::MakeSphere(InteractRadius);
	case ERangedTraceShape::Box:    return FCollisionShape::MakeBox(FVector(InteractRadius));
	case ERangedTraceShape::Capsule:return FCollisionShape::MakeCapsule(InteractRadius, InteractHalfHeight);
	default:                        return FCollisionShape::MakeSphere(InteractRadius);
	}
}

TArray<AActor*> UInteractComponent::GetIgnoredActors() const
{
	TArray<AActor*> IgnoredActors;
	if (bIgnoreOwner)
	{
		IgnoredActors.AddUnique(GetOwner());
	}
	if (bIgnoreChildActors)
	{
		TArray<AActor*> ChildActors;
		GetOwner()->GetAllChildActors(ChildActors);
		IgnoredActors.Append(ChildActors);
	}
	return IgnoredActors;
}
