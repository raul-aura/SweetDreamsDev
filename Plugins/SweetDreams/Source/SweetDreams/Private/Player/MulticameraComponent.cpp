// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MulticameraComponent.h"
#include "Engine/World.h"
#include "Editor.h"
#include "TimerManager.h"

UMulticameraComponent::UMulticameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMulticameraComponent::BeginPlay()
{
	FindCamera();
	Super::BeginPlay();
}

void UMulticameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMulticameraComponent::UpdateCameraView()
{
#if WITH_EDITOR
	if (GIsEditor)
	{
		AActor* Owner = GetOwner();
		if (IsValid(Owner))
		{
			ActiveCamera = Cast<UCameraComponent>(
				Owner->GetComponentByClass(UCameraComponent::StaticClass())
			);
		}
	}
#endif
	if (!IsValid(ActiveCamera) || !CameraViews.IsValidIndex(CurrentView)) return;
	FVector NewLocation = CameraViews[CurrentView].Location;
	FRotator NewRotation = CameraViews[CurrentView].Rotation;
	ActiveCamera->SetRelativeLocationAndRotation(NewLocation, NewRotation);
}

void UMulticameraComponent::SetNewCameraView(int32 CameraToMatch, float BlendTime)
{
	if (CameraViews.Num() == 0 || !CameraViews.IsValidIndex(CameraToMatch) || !IsValid(ActiveCamera))
	{
		return;
	}
	CurrentView = CameraToMatch;
	if (BlendTime <= 0.0f)
	{
		BlendTime = GetWorld()->GetDeltaSeconds();
	}
	if (!IsValid(ActiveCamera)) return;
	StartLocation = ActiveCamera->GetRelativeLocation();
	StartRotation = ActiveCamera->GetRelativeRotation();
	EndLocation = CameraViews[CameraToMatch].Location;
	EndRotation = CameraViews[CameraToMatch].Rotation;
	BlendElapsedTime = 0.0f;
	BlendTotalTime = BlendTime;
	GetOwner()->GetWorldTimerManager().SetTimer(BlendHandle, this, &UMulticameraComponent::CameraBlend, GetWorld()->GetDeltaSeconds(), true);
}

void UMulticameraComponent::SetActiveCamera(UPARAM(ref) UCameraComponent*& NewCamera)
{
	ActiveCamera = NewCamera;
}

TArray<FCameraViews> UMulticameraComponent::GetAllPossibleViews() const
{
	return CameraViews;
}

void UMulticameraComponent::FindCamera() 
{
	ActiveCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
}

void UMulticameraComponent::CameraBlend()
{
	if (BlendTotalTime <= 0.0f)
	{
		return;
	}
	BlendElapsedTime += GetWorld()->GetDeltaSeconds();
	float BlendAlpha = FMath::Clamp(BlendElapsedTime / BlendTotalTime, 0.0f, 1.0f);
	if (IsValid(AlphaCurve) && AlphaCurve->FloatCurve.GetNumKeys() > 0)
	{
		float CurveMin, CurveMax;
		AlphaCurve->GetTimeRange(CurveMin, CurveMax);
		float CurveAlpha = FMath::Lerp(CurveMin, CurveMax, BlendAlpha);
		BlendAlpha = AlphaCurve->GetFloatValue(CurveAlpha);
	}
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, BlendAlpha);
	FRotator NewRotation = FMath::Lerp(StartRotation, EndRotation, BlendAlpha);
	ActiveCamera->SetRelativeLocationAndRotation(NewLocation, NewRotation);
	if (BlendAlpha >= 1.0f) 
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(BlendHandle);
	}
}
