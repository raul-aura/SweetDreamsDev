

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIStateMachineComponent.generated.h"

class USweetDreamsAIState;
class USweetDreamsAIStateBehaviour;

UCLASS( ClassGroup=("SweetDreams"), meta=(BlueprintSpawnableComponent) )
class SWEETDREAMS_API UAIStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAIStateMachineComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|AI")
	bool AddState(USweetDreamsAIState* InState, USweetDreamsAIStateBehaviour*& Behaviour, int32& Index, bool bEnterStateOnAdd = false);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|AI")
	bool SetState(USweetDreamsAIState* InState, USweetDreamsAIStateBehaviour*& Behaviour);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|AI")
	void ClearCurrentState();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	TArray<USweetDreamsAIState*> InitialStates;
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TArray<USweetDreamsAIState*> States;
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TMap<FName, USweetDreamsAIStateBehaviour*> Behaviours;
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	USweetDreamsAIState* CurrentState = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	USweetDreamsAIStateBehaviour* CurrentBehaviour = nullptr;
	
};
