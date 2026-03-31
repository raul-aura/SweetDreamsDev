

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SweetDreamsAIStateBehaviour.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class SWEETDREAMSAI_API USweetDreamsAIStateBehaviour : public UObject
{
	GENERATED_BODY()

public:

	// Called by State Machine Component when this behaviour is constructed. 
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|AI")
	void OnInitialized();

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|AI")
	void OnEnter();

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|AI")
	void OnExit();

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|AI")
	void Tick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|AI")
	bool CanEnterState() const;
	virtual bool CanEnterState_Implementation() const;
	
	void SetOwner(AActor* InOwner);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|AI")
	AActor* GetOwner() const;

	void SetState(USweetDreamsAIState* Data);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|AI")
	USweetDreamsAIState* GetStateData() const;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "AI Behaviour")
	bool bIsStateActive = false;

private:

	TObjectPtr<AActor> StateOwner;

	TObjectPtr<USweetDreamsAIState> StateData;
};

