// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SweetDreamsPlayerController.h"
#include "SweetDreamsPlayerControllerPlus.generated.h"

class ASweetDreamsHUD;

UCLASS()
class SWEETDREAMS_API ASweetDreamsPlayerControllerPlus : public ASweetDreamsPlayerController
{
	GENERATED_BODY()
	
public:
	ASweetDreamsPlayerControllerPlus();
	
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	ASweetDreamsHUD* GetSweetDreamsHUD() const;
	
};
