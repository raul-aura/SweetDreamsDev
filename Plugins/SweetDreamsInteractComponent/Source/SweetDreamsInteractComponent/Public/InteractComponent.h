// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

struct FOverlapResult;

UENUM(BlueprintType)
enum class ERangedTraceShape : uint8
{
	Sphere   UMETA(DisplayName = "Sphere"),
	Box      UMETA(DisplayName = "Box"),
	Capsule  UMETA(DisplayName = "Capsule")
};

UCLASS(ClassGroup = ("SweetDreams"), meta = (BlueprintSpawnableComponent))
class SWEETDREAMSINTERACTCOMPONENT_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Interact")
	TArray<AActor*> FindInteractablesInRange();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Interact")
	AActor* FindInteractableTraced();

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Interact")
	bool InteractTraced(AActor*& ActorTraced);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Interact")
	bool InteractRanged(int32 Index, TArray<AActor*>& ActorsFound);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Interact", meta = (DisplayName = "Interact With Actor"))
	bool InteractActor(AActor* Interactable);

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Core|Interact")
	void OnInteract(AActor* Interactable);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Core|Interact")
	void OnInteractableTraced(AActor* Interactable);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Core|Interact")
	void OnInteractablesFoundRange(const TArray<AActor*>& Interactables);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact")
	bool bUseTracedInteraction = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact")
	bool bUseRangedInteraction = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact")
	bool bLimitToInteractableInterface = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact")
	bool bIgnoreOwner = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact")
	bool bIgnoreChildActors = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Traced", meta = (EditCondition = "bUseTracedInteraction"))
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Traced", meta = (EditCondition = "bUseTracedInteraction"))
	float InteractTraceDistance = 600.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Traced", meta = (EditCondition = "bUseTracedInteraction"))
	bool bDrawDebugLine = false;
	UPROPERTY(BlueprintReadOnly, Category = "Interact")
	TObjectPtr<AActor> ActorTraceHit;
	void InvalidateActorTraced();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Ranged", meta = (EditCondition = "bUseRangedInteraction"))
	ERangedTraceShape RangedShape = ERangedTraceShape::Sphere;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Ranged", meta = (EditCondition = "bUseRangedInteraction"))
	TEnumAsByte<ECollisionChannel> RangedChannel = ECC_Visibility;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Ranged", meta = (EditCondition = "bUseRangedInteraction"))
	float InteractRadius = 600.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Ranged", meta = (EditCondition = "RangedShape==ERangedTraceShape::Capsule", EditConditionHides))
	float InteractHalfHeight = 300.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Ranged", meta = (EditCondition = "bUseRangedInteraction"))
	FVector RangeOrigin = FVector::Zero();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Ranged", meta = (EditCondition = "bUseRangedInteraction"))
	bool bDrawDebugRanged = false;
	UPROPERTY(BlueprintReadOnly, Category = "Interact")
	TArray<TObjectPtr<AActor>> ActorsWithinRange;
	void InvalidateActorsInRange(const TArray<TObjectPtr<AActor>>& PreviousActors);
	FCollisionShape MakeRangedShape() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Core|Interact")
	FVector GetRangeOriginUpdated() const;
	FVector GetRangeOriginUpdated_Implementation() const;

	UPROPERTY(BlueprintReadWrite, Category = "Automatic Find")
	bool bAutoFindTrace = false;
	UPROPERTY(BlueprintReadWrite, Category = "Automatic Find", meta = (EditCondition = "bAutoFindTrace", EditConditionHides))
	float FindTracedInterval = 0.2f;
	UPROPERTY(BlueprintReadOnly, Category = "Interact")
	FTimerHandle FindTracedTimer;
	UPROPERTY(BlueprintReadWrite, Category = "Automatic Find")
	bool bAutoFindRange = false;
	UPROPERTY(BlueprintReadWrite, Category = "Automatic Find", meta = (EditCondition = "bAutoFindRange", EditConditionHides))
	float FindRangeInterval = 0.2f;
	UPROPERTY(BlueprintReadOnly, Category = "Interact")
	FTimerHandle FindRangeTimer;

	TArray<TObjectPtr<AActor>> GetIgnoredActors() const;
};

