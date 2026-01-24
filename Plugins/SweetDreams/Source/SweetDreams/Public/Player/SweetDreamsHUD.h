// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SweetDreamsHUD.generated.h"

class ULoadingWidget;
class USweetDreamsWidget;

UCLASS()
class SWEETDREAMS_API ASweetDreamsHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void CreateStartingWidgets();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player", meta = (DeterminesOutputType = "WidgetClass"))
	USweetDreamsWidget* CreateAndStoreWidget(TSubclassOf<USweetDreamsWidget> WidgetClass);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	virtual void ShowWidget(USweetDreamsWidget* Widget);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	virtual void HideWidget(USweetDreamsWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void OverridePlayerInputMode(USweetDreamsWidget* WidgetToFocus);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void UpdatePlayerInputMode();
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	ULoadingWidget* CreateLoadingWidget(TSubclassOf<ULoadingWidget> WidgetClass);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void ShowLoadingWidget();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void HideLoadingWidget();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	void DestroyLoadingWidget();
	void StartLoadingWidget();
	void FinishLoadingWidget();
	void BeginPlayNewLevelLoadingWidget();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	ULoadingWidget* GetLoadingWidget() const;
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	virtual bool IsAnyWidgetVisible() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	USweetDreamsWidget* GetHighestPriorityWidget() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	USweetDreamsWidget* FindWidgetByClass(TSubclassOf<USweetDreamsWidget> WidgetClass) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	USweetDreamsWidget* FindWidgetByName(FName WidgetName) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Player")
	TArray<USweetDreamsWidget*> GetAllWidgets() const { return AllWidgets; }

protected:
	virtual void PostInitializeComponents() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams HUD")
	TArray<TSubclassOf<USweetDreamsWidget>> StartingWidgets;

	UPROPERTY(BlueprintReadOnly, Category = "Loading Screen")
	TObjectPtr<ULoadingWidget> LoadingWidget = nullptr;

private:
	TArray<USweetDreamsWidget*> AllWidgets;
};

