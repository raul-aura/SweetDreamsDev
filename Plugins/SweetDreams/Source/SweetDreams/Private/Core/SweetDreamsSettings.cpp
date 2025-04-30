// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SweetDreamsSettings.h"
#include "Save/SweetDreamsSaveLocal.h"
#include "Save/SweetDreamsSavePersistent.h"

USweetDreamsSettings::USweetDreamsSettings()
{
	DebugFlags |= static_cast<uint8>(EDebugFlags::PrintEnabled);
	DebugFlags |= static_cast<uint8>(EDebugFlags::PrintSaveOperations);
	PersistentClass = USweetDreamsSavePersistent::StaticClass();
	LocalClass = USweetDreamsSaveLocal::StaticClass();
}