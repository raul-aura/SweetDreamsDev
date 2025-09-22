// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SweetDreamsAIState.generated.h"

class USweetDreamsAIStateBehaviour;

UCLASS()
class SWEETDREAMS_API USweetDreamsAIState : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI State")
	FName StateName = TEXT("State");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI State")
	TSubclassOf<USweetDreamsAIStateBehaviour> BehaviourClass = nullptr;
};
