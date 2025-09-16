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
	USweetDreamsWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Widget")
	FName WidgetName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Widget")
	int32 InitialZOrder = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Widget")
	bool bIgnoreThisForVisibility = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Widget")
	EInputMode InputMode;

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

