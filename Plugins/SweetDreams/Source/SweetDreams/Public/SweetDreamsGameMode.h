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
	ULoadingWidget* CreateLoadingWidget(TSubclassOf<ULoadingWidget> Class, bool bAddToViewport);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core")
	void LevelLoadStarted(TSoftObjectPtr<UWorld> LoadingLevel);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core")
	void LevelLoadFinished(TSoftObjectPtr<UWorld> LoadingLevel);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	USweetDreamsCore* Core = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	TSubclassOf<ULoadingWidget> LoadingWidgetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	bool bShowLoadingScreenOnBeginPlay = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core")
	bool bHideLoadingScreenOnFinish = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Core", meta = (ClampMin = "0"))
	float LoadingDelay = 3.f;
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	ULoadingWidget* LoadingWidget = nullptr;

};
