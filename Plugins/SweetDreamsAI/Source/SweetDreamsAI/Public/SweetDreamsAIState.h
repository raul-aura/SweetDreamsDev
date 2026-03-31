// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SweetDreamsAIState.generated.h"

class USweetDreamsAIStateBehaviour;

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMSAI_API USweetDreamsAIState : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI State")
	FGameplayTag StateTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI State")
	FText DisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "AI State")
	TObjectPtr<USweetDreamsAIStateBehaviour> Behaviour = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Duration", meta = (EditCondition="!bUseRandomizedDuration"))
	float Duration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Duration")
	bool bUseRandomizedDuration = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Duration", meta = (EditCondition = "bUseRandomizedDuration"))
	float MinDuration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Duration", meta = (EditCondition = "bUseRandomizedDuration"))
	float MaxDuration = 1.f;
};
