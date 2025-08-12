// Copyright Epic Games, Inc. All Rights Reserved.

#include "DreamScapeGeneratorEdMode.h"
#include "DreamScapeGeneratorEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FDreamScapeGeneratorEdMode::EM_DreamScapeGeneratorEdModeId = TEXT("EM_DreamScapeGeneratorEdMode");

FDreamScapeGeneratorEdMode::FDreamScapeGeneratorEdMode()
{

}

FDreamScapeGeneratorEdMode::~FDreamScapeGeneratorEdMode()
{

}

void FDreamScapeGeneratorEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FDreamScapeGeneratorEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FDreamScapeGeneratorEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FDreamScapeGeneratorEdMode::UsesToolkits() const
{
	return true;
}




