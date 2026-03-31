
#include "BattleEvents/BattleEventSpawnActor.h"

void UBattleEventSpawnActor::ExecuteEvent_Implementation()
{
	if (SpawnDelay > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnDelayTimer, this, &UBattleEventSpawnActor::SpawnActor, SpawnDelay);
	}
	else
	{
		SpawnActor();
	}
}

bool UBattleEventSpawnActor::IsFinished_Implementation() const
{
	return IsValid(SpawnedActor);
}

FTransform UBattleEventSpawnActor::GetTransform_Implementation() const
{
	return Transform;
}

void UBattleEventSpawnActor::SpawnActor()
{
	if (BattleContext && ActorClass)
	{
		SpawnedActor = BattleContext->SpawnActor(ActorClass, GetTransform());
		
		if (SpawnedActor)
		{
			OnActorSpawned(SpawnedActor);
		}
	}
}
