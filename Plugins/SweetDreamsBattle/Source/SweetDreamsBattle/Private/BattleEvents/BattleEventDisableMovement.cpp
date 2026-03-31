
#include "BattleEvents/BattleEventDisableMovement.h"
#include "Battle/BattleActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBattleEventDisableMovement::ExecuteEvent_Implementation()
{
	CachedTargets = BattleContext->GetSelectedTargets(TargetSettings);

	for (const UBattleActorComponent* Target : CachedTargets)
	{
		AActor* Owner = Target->GetOwner();
		if (!Owner) continue;

		if (UCharacterMovementComponent* CharMove = Owner->FindComponentByClass<UCharacterMovementComponent>())
		{
			CharMove->StopMovementImmediately();
			CharMove->DisableMovement();
		}
		else if (UMovementComponent* Move = Owner->FindComponentByClass<UMovementComponent>())
		{
			Move->StopMovementImmediately();
		}

		if (APawn* Pawn = Cast<APawn>(Owner))
		{
			if (AController* Controller = Pawn->GetController())
			{
				Controller->StopMovement();
			}
		}
	}

	if (bUseTimedDuration)
	{
		bTimerExecuted = false;
		GetWorld()->GetTimerManager().SetTimer(ReenableTimer, this, &UBattleEventDisableMovement::ReenableMovement, Duration, false);
	}
}

bool UBattleEventDisableMovement::IsFinished_Implementation() const
{
	if (bUseTimedDuration)
	{
		return bTimerExecuted;
	}

	return true;
}

void UBattleEventDisableMovement::ReenableMovement()
{
	for (const UBattleActorComponent* Target : CachedTargets)
	{
		AActor* Owner = Target->GetOwner();
		if (!Owner) continue;

		if (UCharacterMovementComponent* CharMove = Owner->FindComponentByClass<UCharacterMovementComponent>())
		{
			CharMove->SetDefaultMovementMode();
		}
	}

	bTimerExecuted = true;
}
