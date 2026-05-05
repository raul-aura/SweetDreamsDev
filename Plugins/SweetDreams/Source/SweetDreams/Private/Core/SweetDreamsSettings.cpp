// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SweetDreamsSettings.h"
#include "Save/SweetDreamsSaveFile.h"

USweetDreamsSettings::USweetDreamsSettings()
{
	PersistentClass = USweetDreamsSaveFile::StaticClass();
}