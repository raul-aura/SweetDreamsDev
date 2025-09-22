

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SweetDreamsAIStateBehaviour.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class SWEETDREAMS_API USweetDreamsAIStateBehaviour : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Core|AI")
	bool OnEnter();
	virtual bool OnEnter_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Core|AI")
	void OnExit();

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Core|AI")
	void Tick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|Core|AI")
	bool CanEnterState() const;
	virtual bool CanEnterState_Implementation() const;
	
	void SetOwner(AActor* InOwner);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|AI")
	AActor* GetOwner() const;


protected:

	UPROPERTY(BlueprintReadOnly)
	AActor* StateOwner;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStateActive = false;

};

