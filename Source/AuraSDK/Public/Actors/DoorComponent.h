

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DoorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorDelegateSignature);

UENUM(BlueprintType)
enum class EPlayerDirection : uint8
{
	Parallel, 
	Opposite
};

UCLASS(ClassGroup = ("AuraSDK"), meta = (BlueprintSpawnableComponent))
class AURASDK_API UDoorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UDoorComponent();

	UFUNCTION(BlueprintCallable, Category = "AuraSDK|Door Component")
	void InteractDoor();
	UFUNCTION(BlueprintCallable, Category = "AuraSDK|Door Component")
	void OpenDoor();
	UFUNCTION(BlueprintCallable, Category = "AuraSDK|Door Component")
	void CloseDoor();
	UFUNCTION(BlueprintCallable, Category = "AuraSDK|Door Component")
	void ToggleDoorLock(bool bInUnlocked = true);

	UFUNCTION(BlueprintCallable, Category = "AuraSDK|Door Component")
	bool TeleportToPoint(FName InPoint);

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "AuraSDK|Door Component")
	EPlayerDirection GetPlayerDirection() const;

	UPROPERTY(BlueprintAssignable, Category = "AuraSDK|Door Component")
	FOnDoorDelegateSignature OnDoorInteracted;
	UPROPERTY(BlueprintAssignable, Category = "AuraSDK|Door Component")
	FOnDoorDelegateSignature OnDoorOpened;
	UPROPERTY(BlueprintAssignable, Category = "AuraSDK|Door Component")
	FOnDoorDelegateSignature OnDoorClosed;
	UPROPERTY(BlueprintAssignable, Category = "AuraSDK|Door Component")
	FOnDoorDelegateSignature OnDoorUnlocked;
	UPROPERTY(BlueprintAssignable, Category = "AuraSDK|Door Component")
	FOnDoorDelegateSignature OnDoorLocked;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Door Component")
	bool bIsUnlocked = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Door Component")
	bool bIsOpen = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Door Component|Teleport")
	TMap<FName, FVector> TeleportPoints;
	
};
