

#include "BattleEvents/BattleEventDelay.h"

void UBattleEventDelay::ExecuteEvent_Implementation()
{
	TimeElapsed = 0.f;
}

bool UBattleEventDelay::IsFinished_Implementation() const
{
	return TimeElapsed >= Duration;
}

void UBattleEventDelay::Tick_Implementation(float DeltaTime)
{
	if (TimeElapsed < Duration)
	{
		TimeElapsed += DeltaTime;
	}
}
