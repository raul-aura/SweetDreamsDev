// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/SweetDreamsSaveFile.h"
#include "EngineUtils.h"

USweetDreamsSaveFile::USweetDreamsSaveFile() {}

void USweetDreamsSaveFile::UpdateCurrentVersion()
{
	CurrentVersion = SavedDeltaData.Num() - 1;
}
