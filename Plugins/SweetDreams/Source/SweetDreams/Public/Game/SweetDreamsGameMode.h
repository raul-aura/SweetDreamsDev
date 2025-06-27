// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SweetDreamsGameMode.generated.h"

class USweetDreamsCore;
class ULoadingWidget;
struct FDreamUserSettings;

UCLASS()
class SWEETDREAMS_API ASweetDreamsGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	ASweetDreamsGameMode();

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core")
	ULoadingWidget* CreateLoadingWidget(TSubclassOf<ULoadingWidget> Class, bool bAddToViewport);
	void LevelLoadStarted(TSoftObjectPtr<UWorld> LoadingLevel);
	void LevelLoadFinished(TSoftObjectPtr<UWorld> LoadingLevel);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Core")
	USweetDreamsCore* SweetDreamsCore = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen")
	TSubclassOf<ULoadingWidget> LoadingWidgetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen")
	bool bShowOnBeginPlay = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen", meta = (ClampMin = "0"))
	float WidgetGracePeriod = 3.f; // grace period for loading widget, called on begin play 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen")
	bool bHideAfterGracePeriod = true; // when grace period ends, should auto hide the widget?
	UPROPERTY(BlueprintReadOnly, Category = "Loading Screen")
	ULoadingWidget* LoadingWidget = nullptr;

};
