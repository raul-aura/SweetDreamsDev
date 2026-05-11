

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CheckpointComponent.generated.h"

class USweetDreamsSaveFile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckpointDelegate);

UCLASS( ClassGroup=("AuraSDK"), meta=(BlueprintSpawnableComponent) )
class AURASDK_API UCheckpointComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCheckpointComponent();
	
	UFUNCTION(BlueprintPure, Category = "AuraSDK | Checkpoint Component", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static UCheckpointComponent* FindCheckpointByID(const UObject* WorldContextObject, FName Identifier);

	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Checkpoint Component")
	void UpdateCheckpointRespawn(FVector Location, FRotator Rotation);

	// Attempts to save persistent game, if not successful, will attempt to create the persistent save file using SweetDreamsCore.
	UFUNCTION(BlueprintCallable, Category = "AuraSDK | Checkpoint Component")
	void CheckpointSaveGame(UPARAM(ref) bool& bExecutedSaveOperation);

	UPROPERTY(BlueprintAssignable, Category = "Checkpoint Component")
	FOnCheckpointDelegate OnGameSaved;
	UPROPERTY(BlueprintAssignable, Category = "Checkpoint Component")
	FOnCheckpointDelegate OnCheckpointFirstInteracted;

protected:

	virtual void BeginPlay() override;
	
	// If auto create save is disabled, the CheckpointSaveGame() function will create a persistent save file of this class.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Checkpoint Component")
	TSubclassOf<USweetDreamsSaveFile> CustomSaveClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Checkpoint Component")
	FName CheckpointID;

	UPROPERTY(BlueprintReadOnly, Category = "Checkpoint Component")
	FVector RespawnLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Checkpoint Component")
	FRotator RespawnRotation;
};
