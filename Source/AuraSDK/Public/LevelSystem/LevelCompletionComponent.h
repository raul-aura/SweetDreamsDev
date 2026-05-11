

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelCompletionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelExit);

UCLASS( ClassGroup=(AuraSDK), meta=(BlueprintSpawnableComponent) )
class AURASDK_API ULevelCompletionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULevelCompletionComponent();

	UFUNCTION(BlueprintCallable)
	void ExitLevel();
	UFUNCTION(BlueprintCallable)
	void ToggleCanExitLevel(bool bInCanExitLevel = true);

	UPROPERTY(BlueprintAssignable)
	FOnLevelExit OnLevelExit;

protected:
	virtual void BeginPlay() override;

	void RegisterToLevelManager();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bCanExitLevel = false;
	
};