

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SweetDreamsAIStateBehaviour.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class SWEETDREAMSAI_API USweetDreamsAIStateBehaviour : public UObject
{
	GENERATED_BODY()

public:

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


protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> StateOwner;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStateActive = false;

};

