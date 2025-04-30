// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SweetDreamsHUD.generated.h"

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	GAMEANDUI = 0 UMETA(DisplayName = "Game and UI"),
	GAME = 1 UMETA(DisplayName = "Game"),
	UI = 2 UMETA(DisplayName = "UI")
};

UCLASS()
class SWEETDREAMS_API ASweetDreamsHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateWidgets();
	UFUNCTION(BlueprintCallable)
	void CreateAndStoreWidget(TSubclassOf<USweetDreamsWidget> WidgetClass);
	UFUNCTION(BlueprintCallable)
	static void ShowWidget(USweetDreamsWidget* Widget);
	UFUNCTION(BlueprintCallable)
	static void HideWidget(USweetDreamsWidget* Widget);
	UFUNCTION(BlueprintPure)
	static USweetDreamsWidget* FindWidgetByClass(TSubclassOf<USweetDreamsWidget> WidgetClass);
	UFUNCTION(BlueprintPure)
	static USweetDreamsWidget* FindWidgetByName(FName WidgetName);
	UFUNCTION(BlueprintCallable)
	static void UpdatePlayerInputMode(USweetDreamsWidget* WidgetToFocus);
	UFUNCTION(BlueprintPure)
	static bool IsAnyWidgetVisible();
	UFUNCTION(BlueprintPure)
	static TArray<USweetDreamsWidget*> GetAllWidgets() { return AllWidgets; }


protected:
	virtual void PostInitializeComponents() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<USweetDreamsWidget>> DefaultWidgets;

private:
	static TArray<USweetDreamsWidget*> AllWidgets;
};

