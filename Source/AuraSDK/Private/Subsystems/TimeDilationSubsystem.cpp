
#include "Subsystems/TimeDilationSubsystem.h"
#include "Curves/CurveFloat.h"

void UTimeDilationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTimeDilationSubsystem::Tick(float DeltaTime)
{
	for (FTimeDilationOperation& Operation : Operations)
	{
		Operation.TimeElapsed += DeltaTime;

		float Alpha = FMath::Clamp(Operation.TimeElapsed / Operation.Duration, 0.f, 1.f);
		float CurveValue = Operation.Curve ? Operation.Curve->GetFloatValue(Alpha) : 1.f;

		for (const FTimeDilationOperationTarget& Target : Operation.Actors)
		{
			float CurrentDilation = FMath::Lerp(Target.PreviousTimeDilation, Operation.TimeDilation, CurveValue);
			Target.Actor->CustomTimeDilation = CurrentDilation;
		}
	}

	Operations.RemoveAll([&](const FTimeDilationOperation& Op)
	{
		if (Op.TimeElapsed >= Op.Duration)
		{
			for (const FTimeDilationOperationTarget& Target : Op.Actors)
			{
				Target.Actor->CustomTimeDilation = Target.PreviousTimeDilation;
			}

			return true;
		}

		return false;
	});
}

FTimeDilationOperation UTimeDilationSubsystem::LerpTimeDilation(const TArray<AActor*>& Actors, float TimeDilation, float Duration, UCurveFloat* Curve)
{
	FTimeDilationOperation Operation;
	if (Actors.IsEmpty()) return Operation;

	for (AActor* Actor : Actors)
	{
		Operation.Actors.Add(FTimeDilationOperationTarget(Actor, Actor->GetActorTimeDilation()));
	}

	Operation.TimeDilation = TimeDilation;
	Operation.Duration = Duration;
	Operation.Curve = Curve;

	Operations.Add(Operation);

	return Operation;
}
