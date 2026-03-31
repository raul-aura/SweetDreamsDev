// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractComponent.h"
#include "SweetDreamsInteractInterface.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	RangeOrigin = GetRangedOrigin();

	if (bAutoFindTrace)
	{
		GetWorld()->GetTimerManager().SetTimer(
			FindTracedTimer,
			[this]()
			{
				FindInteractableTraced();
			},
			FindTracedInterval, true
		);
	}

	if (bAutoFindRange)
	{
		GetWorld()->GetTimerManager().SetTimer(
			FindRangeTimer,
			[this]()
			{
				FindInteractablesInRange();
			},
			FindRangeInterval, true
		);
	}
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDrawDebugRanged)
	{
		DrawDebugSphere(GetWorld(), RangeOrigin, RangedRadius, 12, FColor::Green, false, -1.f, 0, 5.f);
	}

	if (bDrawDebugLine && IsValid(GetOwner()))
	{
		UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();

		if (!IsValid(Camera)) return;

		FVector Start = Camera->GetComponentLocation();
		FVector End = Start + (Camera->GetForwardVector() * TraceDistance);
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 5.f);
	}
}

TArray<AActor*> UInteractComponent::FindInteractablesInRange()
{
	TArray<TObjectPtr<AActor>> PreviousActors = ActorsWithinRange;
	ActorsWithinRange.Reset();

	RangeOrigin = GetRangedOrigin();
	FCollisionShape Shape = MakeRangedShape();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(GetIgnoredActors());
	TArray<FOverlapResult> Overlaps;
	bool bHasOverlaps = GetWorld()->OverlapMultiByChannel(
		Overlaps, RangeOrigin, FQuat::Identity, RangedChannel, Shape, QueryParams
	);

	TSet<TObjectPtr<AActor>> UniqueOverlaps;

	if (bHasOverlaps)
	{
		for (const FOverlapResult& Result : Overlaps)
		{
			TObjectPtr<AActor> HitActor = Result.GetActor();

			if (IsValid(HitActor))
			{
				if (IsActorInteractable(HitActor))
				{
					UniqueOverlaps.Add(HitActor);
					if (!PreviousActors.Contains(HitActor) && HitActor->Implements<USweetDreamsInteractInterface>())
					{
						ISweetDreamsInteractInterface::Execute_OnEnterInteractRange(HitActor, GetOwner());
					}
				}
			}
		}
	}

	ActorsWithinRange = UniqueOverlaps.Array();
	InvalidateActorsInRange(PreviousActors);
	OnInteractablesInRange.Broadcast(ActorsWithinRange);
	return ActorsWithinRange;
}

AActor* UInteractComponent::FindInteractableTraced()
{
	if (!IsValid(GetOwner())) return nullptr;

	TObjectPtr<UCameraComponent> Camera = GetOwner()->FindComponentByClass<UCameraComponent>();

	if (!IsValid(Camera)) return nullptr;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(GetIgnoredActors());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, QueryParams);
	if (bHit)
	{
		TObjectPtr<AActor> HitActor = HitResult.GetActor();

		if (IsValid(HitActor))
		{
			if (IsActorInteractable(HitActor))
			{
				if (ActorTraceHit != HitActor)
				{
					InvalidateActorTraced();
					ActorTraceHit = HitActor;
					OnInteractableTraced.Broadcast(HitActor);
					if (HitActor->Implements<USweetDreamsInteractInterface>())
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
		OnInteract.Broadcast(Interactable);

		return true;
	}

	return false;
}

void UInteractComponent::InvalidateActorTraced()
{
	if (IsValid(ActorTraceHit))
	{
		OnTracedInteractableInvalidated.Broadcast();

		if (ActorTraceHit->Implements<USweetDreamsInteractInterface>())
		{
			ISweetDreamsInteractInterface::Execute_OnEndTrace(ActorTraceHit, GetOwner());
		}
	}

	ActorTraceHit = nullptr;
}

void UInteractComponent::InvalidateActorsInRange(const TArray<TObjectPtr<AActor>>& PreviousActors)
{
	if (ActorsWithinRange != PreviousActors)
	{
		OnRangedInteractablesInvalidated.Broadcast();
	}

	for (TObjectPtr<AActor> PrevActor : PreviousActors)
	{
		if (!ActorsWithinRange.Contains(PrevActor) && IsValid(PrevActor) && PrevActor->Implements<USweetDreamsInteractInterface>())
		{
			ISweetDreamsInteractInterface::Execute_OnExitInteractRange(PrevActor, GetOwner());
		}
	}
}

FCollisionShape UInteractComponent::MakeRangedShape() const
{
	switch (RangedShape)
	{
	case ERangedTraceShape::Sphere: return FCollisionShape::MakeSphere(RangedRadius);
	case ERangedTraceShape::Box:    return FCollisionShape::MakeBox(FVector(RangedRadius));
	case ERangedTraceShape::Capsule:return FCollisionShape::MakeCapsule(RangedRadius, RangedHalfHeight);
	default:                        return FCollisionShape::MakeSphere(RangedRadius);
	}
}

FVector UInteractComponent::GetRangedOrigin_Implementation() const
{
	if (!IsValid(GetOwner())) return FVector::Zero();

	return GetOwner()->GetActorLocation();
}

TArray<TObjectPtr<AActor>> UInteractComponent::GetIgnoredActors() const
{
	TArray<TObjectPtr<AActor>> IgnoredActors;

	if (IsValid(GetOwner()))
	{
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
	}

	return IgnoredActors;
}

bool UInteractComponent::IsClassAccepted(const AActor* Actor) const
{
	if (!IsValid(Actor))
	{
		return false;
	}

	const UClass* ActorClass = Actor->GetClass();

	if (!FilteredClasses.IsEmpty())
	{
		for (const TSubclassOf<AActor> Class : FilteredClasses)
		{
			if (ActorClass->IsChildOf(Class))
			{
				return true;
			}
		}

		return false;
	}

	for (const TSubclassOf<AActor> Class : ExcludedClasses)
	{
		if (ActorClass->IsChildOf(Class))
		{
			return false;
		}
	}

	return true;
}

bool UInteractComponent::IsActorInteractable(const AActor* Actor) const
{
	if (!IsValid(Actor))
	{
		return false;
	}

	if (bLimitToInteractableInterface)
	{
		return Actor->Implements<USweetDreamsInteractInterface>();
	}

	return IsClassAccepted(Actor);
}
