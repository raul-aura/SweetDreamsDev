
#include "Actors/DoorComponent.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"

UDoorComponent::UDoorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDoorComponent::BeginPlay()
{
	if (!bIsUnlocked) return;
	bIsOpen ? OpenDoor() : CloseDoor();
}

void UDoorComponent::InteractDoor()
{
	OnDoorInteracted.Broadcast();

	if (bIsUnlocked)
	{
		bIsOpen = !bIsOpen;

		bIsOpen ? OnDoorOpened.Broadcast() : OnDoorClosed.Broadcast();
	}
}

void UDoorComponent::OpenDoor()
{
	bIsOpen = true;
	OnDoorOpened.Broadcast();
}

void UDoorComponent::CloseDoor()
{
	bIsOpen = false;
	OnDoorClosed.Broadcast();
}

void UDoorComponent::ToggleDoorLock(bool bInUnlocked)
{
	if (bIsUnlocked == bInUnlocked) return;

	bIsUnlocked = bInUnlocked;

	bIsUnlocked ? OnDoorUnlocked.Broadcast() : OnDoorLocked.Broadcast();
}

bool UDoorComponent::TeleportToPoint(FName InPoint)
{
	const APlayerController* Player = GetWorld()->GetFirstPlayerController();
	if (!Player || !Player->GetCharacter() || !TeleportPoints.Contains(InPoint))
	{
		return false;
	}

	return Player->GetCharacter()->TeleportTo(TeleportPoints[InPoint], Player->GetCharacter()->GetActorRotation());
}

EPlayerDirection UDoorComponent::GetPlayerDirection() const
{
	const APlayerController* Player = GetWorld()->GetFirstPlayerController();
	if (!Player || !Player->GetCharacter())
	{
		return EPlayerDirection::Parallel;
	}

	FVector DoorForward = GetForwardVector();
	DoorForward.Z = 0.f;

	FVector ToPlayer = Player->GetCharacter()->GetActorLocation() - GetComponentLocation();
	ToPlayer.Z = 0.f;

	const float CrossZ = FVector::CrossProduct(DoorForward, ToPlayer).Z;

	if (FMath::Abs(CrossZ) < KINDA_SMALL_NUMBER) return EPlayerDirection::Parallel;

	return CrossZ >= 0.f ? EPlayerDirection::Parallel : EPlayerDirection::Opposite;
}


