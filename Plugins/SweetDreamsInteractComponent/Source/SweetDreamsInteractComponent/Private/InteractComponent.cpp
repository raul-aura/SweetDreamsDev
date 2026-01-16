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
		DrawDebugSphere(GetWorld(), RangeOrigin, InteractRadius, 12, FColor::Green, false, -1.f, 0, 5.f);
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

			if (!IsValid(HitActor)) continue;

			const bool bIsInteractable = (bLimitToInteractableInterface && HitActor->Implements<USweetDreamsInteractInterface>()) || IsClassAccepted(HitActor);
			if (bIsInteractable)
			{
				UniqueOverlaps.Add(HitActor);
				if (!PreviousActors.Contains(HitActor) && HitActor->Implements<USweetDreamsInteractInterface>())
				{
					ISweetDreamsInteractInterface::Execute_OnEnterInteractRange(HitActor, GetOwner());
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
	TObjectPtr<UCameraComponent> Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * InteractTraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(GetIgnoredActors());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, QueryParams);
	if (bHit)
	{
		TObjectPtr<AActor> HitActor = HitResult.GetActor();

		if (IsValid(HitActor))
		{
			const bool bIsInteractable = (bLimitToInteractableInterface && HitActor->Implements<USweetDreamsInteractInterface>()) || IsClassAccepted(HitActor);
			if (bIsInteractable)
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
	if (IsValid(ActorTraceHit) && ActorTraceHit->Implements<USweetDreamsInteractInterface>())
	{
		ISweetDreamsInteractInterface::Execute_OnEndTrace(ActorTraceHit, GetOwner());
	}

	ActorTraceHit = nullptr;
}

void UInteractComponent::InvalidateActorsInRange(const TArray<TObjectPtr<AActor>>& PreviousActors)
{
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
	case ERangedTraceShape::Sphere: return FCollisionShape::MakeSphere(InteractRadius);
	case ERangedTraceShape::Box:    return FCollisionShape::MakeBox(FVector(InteractRadius));
	case ERangedTraceShape::Capsule:return FCollisionShape::MakeCapsule(InteractRadius, InteractHalfHeight);
	default:                        return FCollisionShape::MakeSphere(InteractRadius);
	}
}

FVector UInteractComponent::GetRangedOrigin_Implementation() const
{
	return GetOwner()->GetActorLocation();
}

TArray<TObjectPtr<AActor>> UInteractComponent::GetIgnoredActors() const
{
	TArray<TObjectPtr<AActor>> IgnoredActors;

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

bool UInteractComponent::IsClassAccepted(TObjectPtr<AActor> Actor) const
{
	if (!FilteredClasses.IsEmpty())
	{
		for (const TSubclassOf<AActor> Class : FilteredClasses)
		{
			if (Actor->GetClass()->IsChildOf(Class)) return true;
		}

		return false;
	}

	for (const TSubclassOf<AActor> Class : ExcludedClasses)
	{
		if (Actor->GetClass()->IsChildOf(Class)) return false;
	}

	return true;
}
