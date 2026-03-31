// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SweetDreamsInteractInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USweetDreamsInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class SWEETDREAMSINTERACTCOMPONENT_API ISweetDreamsInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Interact")
	void OnInteract(AActor* InteractionInstigator);

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Interact")
	void OnBeginTrace(AActor* TraceInstigator);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Interact")
	void OnEndTrace(AActor* TraceInstigator);

	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Interact")
	void OnEnterInteractRange(AActor* TraceInstigator);
	UFUNCTION(BlueprintImplementableEvent, Category = "Sweet Dreams|Interact")
	void OnExitInteractRange(AActor* TraceInstigator);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Sweet Dreams|Interact")
	bool IsInteractable() const;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Sweet Dreams|Interact")
	FText GetInteractableText() const;
};

