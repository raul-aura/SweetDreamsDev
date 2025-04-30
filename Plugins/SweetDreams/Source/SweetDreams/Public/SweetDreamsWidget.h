// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SweetDreamsHUD.h"
#include "SweetDreamsWidget.generated.h"

UCLASS()
class SWEETDREAMS_API USweetDreamsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Widget")
	FName WidgetName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Widget")
	int32 InitialZOrder = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Widget")
	bool bIgnoreThisForVisibility = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Widget")
	EInputMode InputMode = EInputMode::UI;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	void OnFirstShow();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	void OnShow();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	void OnHide();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	void HideSelf();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	int32 GetInitialZOrder() const { return InitialZOrder; }
	//
	void HUDShow();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams Widget")
	bool bWidgetShowed = false;

};

