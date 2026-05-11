#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeDilationSubsystem.generated.h"

class UCurveFloat;

USTRUCT()
struct FTimeDilationOperationTarget
{
	GENERATED_BODY()

public:

	AActor* Actor;

	float PreviousTimeDilation = 0.f;

	FTimeDilationOperationTarget() = default;

	FTimeDilationOperationTarget(AActor* InActor, float InPrevious)
		:Actor(InActor),
		PreviousTimeDilation(InPrevious)
	{}
};


USTRUCT(BlueprintType)
struct AURASDK_API FTimeDilationOperation
{
	GENERATED_BODY()

public:

	TArray<FTimeDilationOperationTarget> Actors;

	UPROPERTY(BlueprintReadOnly, Category = "Time Dilation Operation")
	float TimeDilation = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Time Dilation Operation")
	float Duration = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Time Dilation Operation")
	float TimeElapsed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Time Dilation Operation")
	TObjectPtr<UCurveFloat> Curve = nullptr;

	FTimeDilationOperation() = default;
};

UCLASS()
class AURASDK_API UTimeDilationSubsystem final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return Super::GetStatID(); }

	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Time Dilation Subsystem")
	FTimeDilationOperation LerpTimeDilation(const TArray<AActor*>& Actors, float TimeDilation, float Duration, UCurveFloat* Curve);

private:

	TArray<FTimeDilationOperation> Operations;
};
