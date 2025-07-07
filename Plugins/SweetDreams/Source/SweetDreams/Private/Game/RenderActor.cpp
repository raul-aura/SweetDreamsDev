// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/RenderActor.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/DirectionalLightComponent.h"

ARenderActor::ARenderActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SkyLight = CreateDefaultSubobject<USkyLightComponent>("Skylight");
	SkyLight->SetupAttachment(RootComponent);
	SkyLight->SetMobility(EComponentMobility::Movable);
	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>("Sunlight");
	SunLight->SetupAttachment(RootComponent);
	SunLight->SetMobility(EComponentMobility::Movable);
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>("Post Process");
	PostProcess->SetupAttachment(RootComponent);
	PostProcess->bUnbound = true;
	Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>("Fog");
	Fog->SetupAttachment(RootComponent);
}

void ARenderActor::UpdateCurrentRender()
{
	QuickChangeRender(CurrentQuickRender);
}

void ARenderActor::QuickChangeRender(int32 Index)
{
	ChangePostProcessSettings(Index);
	QuickChangeSun(Index);
	QuickChangeFog(Index);
	QuickChangeSky(Index);
}

void ARenderActor::ChangePostProcessSettings(int32 Index)
{
	if (PostProcessSettings.IsValidIndex(Index))
	{
		PostProcess->Settings = PostProcessSettings[Index];
	}
}

void ARenderActor::QuickChangeSky(int32 Index)
{
	if (SkyQuickRender.IsValidIndex(Index))
	{
		SkyLight->Intensity = SkyQuickRender[Index].SkyIntensity;
		SkyLight->LightColor = SkyQuickRender[Index].SkyColor;
		SkyLight->LowerHemisphereColor = SkyQuickRender[Index].LowerHemisphereColor;
		SkyLight->bLowerHemisphereIsBlack = SkyQuickRender[Index].bUseLowerHemisphereColor;
		SkyLight->RecaptureSky();
	}
}

void ARenderActor::QuickChangeSun(int32 Index)
{
	if (SunQuickRender.IsValidIndex(Index))
	{
		SunLight->Intensity = SunQuickRender[Index].SunIntensity;
		SunLight->LightColor = SunQuickRender[Index].SunColor;
		SunLight->SetWorldRotation(SunQuickRender[Index].SunRotation);
	}
}

void ARenderActor::QuickChangeFog(int32 Index)
{
	if (FogQuickRender.IsValidIndex(Index))
	{
		Fog->FogDensity = FogQuickRender[Index].FogDensity;
		Fog->FogInscatteringColor = FogQuickRender[Index].FogColor;
		Fog->SetVolumetricFog(FogQuickRender[Index].bIsVolumetric);
	}
}
