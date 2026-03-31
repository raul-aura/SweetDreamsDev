

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIStateMachineComponent.generated.h"

class USweetDreamsAIState;
class USweetDreamsAIStateBehaviour;

USTRUCT(BlueprintType)
struct SWEETDREAMSAI_API FSweetDreamsStateRuntime
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TObjectPtr<USweetDreamsAIState> Data;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TObjectPtr<USweetDreamsAIStateBehaviour> Behaviour;

	FSweetDreamsStateRuntime() = default;

	FSweetDreamsStateRuntime(USweetDreamsAIState* InData, USweetDreamsAIStateBehaviour* InBehaviour)
		: Data(InData),
		Behaviour(InBehaviour)
	{}
};

UCLASS( ClassGroup=("SweetDreams"), Blueprintable, meta=(BlueprintSpawnableComponent) )
class SWEETDREAMSAI_API UAIStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAIStateMachineComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|AI")
	void CreateInitialStates();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|AI")
	bool AddState(USweetDreamsAIState* StateData, FSweetDreamsStateRuntime& OutState, int32& Index, bool bEnterStateOnAdd = false);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|AI")
	bool SetState(FGameplayTag InStateTag, FSweetDreamsStateRuntime& OutState);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|AI")
	void ClearCurrentState();

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|AI")
	bool GetStateByTag(FGameplayTag InStateTag, FSweetDreamsStateRuntime& OutState) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|AI")
	float GetStateDuration(FGameplayTag InStateTag) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|AI")
	USweetDreamsAIStateBehaviour* GetStateBehaviour(FGameplayTag InStateTag) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|AI")
	bool IsInState(FGameplayTag InStateTag) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|AI")
	void OnInitialStatesCreated();
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|AI")
	bool CanUpdateCurrentState() const;
	virtual bool CanUpdateCurrentState_Implementation() const;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	TArray<TObjectPtr<USweetDreamsAIState>> InitialStates;
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TArray<FSweetDreamsStateRuntime> States;
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	FSweetDreamsStateRuntime CurrentState;
	
};
