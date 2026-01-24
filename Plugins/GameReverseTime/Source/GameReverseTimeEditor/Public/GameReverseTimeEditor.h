// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(GameReverseTimeEditor, Log, All);

class FGameReverseTimeEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<class FExternalEntryWindow> ExternalEntryWindow;
};
