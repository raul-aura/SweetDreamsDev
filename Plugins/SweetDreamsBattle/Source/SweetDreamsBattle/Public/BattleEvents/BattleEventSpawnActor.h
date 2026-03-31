

#pragma once

#include "CoreMinimal.h"
#include "Data/BattleEvent.h"
#include "BattleEventSpawnActor.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API UBattleEventSpawnActor : public UBattleEvent
{
	GENERATED_BODY()

public:

	virtual void ExecuteEvent_Implementation() override;
	virtual bool IsFinished_Implementation() const override;

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Battle|Battle Event Spawn Actor")
	FTransform GetTransform() const;
	FTransform GetTransform_Implementation() const;
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Battle|Battle Event Spawn Actor")
	void OnActorSpawned(AActor* Actor);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawn Actor")
	TSubclassOf<AActor> ActorClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawn Actor")
	FTransform Transform;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawn Actor")
	float SpawnDelay = 0.f;

protected:

	UFUNCTION()
	void SpawnActor();

	UPROPERTY(BlueprintReadOnly, Category = "Spawn Actor")
	TObjectPtr<AActor> SpawnedActor;

	UPROPERTY(BlueprintReadOnly, Category = "Spawn Actor")
	FTimerHandle SpawnDelayTimer;
};
