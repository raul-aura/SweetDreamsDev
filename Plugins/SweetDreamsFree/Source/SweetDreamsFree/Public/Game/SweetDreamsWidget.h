// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "Player/SweetDreamsHUD.h"
#include "SweetDreamsWidget.generated.h"

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	GAMEANDUI = 0 UMETA(DisplayName = "Game and UI"),
	GAME = 1 UMETA(DisplayName = "Game"),
	UI = 2 UMETA(DisplayName = "UI")
};

UCLASS()
class SWEETDREAMSFREE_API USweetDreamsWidget : public UUserWidget
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

	//
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	void OnShow();
	//
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	void OnHide();


	// Changes the visibility 
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	virtual void ShowWidget();
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	virtual void HideWidget();

	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Widget")
	int32 GetInitialZOrder() const { return InitialZOrder; }

};

