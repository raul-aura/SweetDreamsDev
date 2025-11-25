// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SweetDreamsSettings.h"
#include "Save/SweetDreamsSaveFile.h"

USweetDreamsSettings::USweetDreamsSettings()
{
	DebugFlags |= static_cast<uint8>(EDebugFlags::PrintEnabled);
	DebugFlags |= static_cast<uint8>(EDebugFlags::PrintSaveOperations);
	PersistentClass = USweetDreamsSaveFile::StaticClass();
	LocalClass = USweetDreamsSaveFile::StaticClass();
}