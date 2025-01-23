// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsCore.h"
#include "GameFramework/GameModeBase.h"
#include "SweetDreamsGameMode.generated.h"

class USweetDreamsCore;
struct FDreamUserSettings;

UCLASS()
class SWEETDREAMS_API ASweetDreamsGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	ASweetDreamsGameMode();

public:
	virtual void BeginPlay() override;

	// LEVEL LOAD
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core")
	ULoadingWidget* CreateLoadingWidget(TSubclassOf<ULoadingWidget> Class);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|Core")
	float LoadingDelay = 3.f;
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams|Core")
	ULoadingWidget* LoadingWidget;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams|Core")
	USweetDreamsCore* Core = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|Core")
	TSubclassOf<ULoadingWidget> LoadingWidgetClass;
};
