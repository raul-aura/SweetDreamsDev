// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SweetDreamsWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWidgetDelegate, USweetDreamsWidget*, WidgetReference);
DECLARE_DELEGATE_OneParam(FOnRequestSignature, USweetDreamsWidget* /* WidgetReference */);

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	GAMEANDUI = 0 UMETA(DisplayName = "Game and UI"),
	GAME = 1 UMETA(DisplayName = "Game"),
	UI = 2 UMETA(DisplayName = "UI")
};

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

	UPROPERTY(BlueprintAssignable)
	FOnWidgetDelegate OnWidgetShow;
	UPROPERTY(BlueprintAssignable)
	FOnWidgetDelegate OnWidgetHide;

	FOnRequestSignature OnShowRequested;
	FOnRequestSignature OnHideRequested;

	// Implementable event called when this widget changes visibility to Self Hit-Test Invisible by ShowWidget() function.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	void OnShow();
	// Implementable event called when this widget changes visibility to Collapsed by HideWidget() function.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	void OnHide();

	// Changes the visibility of this widget to Self Hit-Test Invisible, without affecting children.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	void ShowWidget();
	// Changes the visibility of this widget to Collapsed.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	void HideWidget();

	// Returns the InitialZOrder variable.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	int32 GetInitialZOrder() const { return InitialZOrder; }

	void ShowWidget_Internal();
	void HideWidget_Internal();
};

