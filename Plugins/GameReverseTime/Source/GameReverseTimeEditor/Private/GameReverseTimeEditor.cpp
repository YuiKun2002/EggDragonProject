// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameReverseTimeEditor.h"
#include "ExternalEntry/ExternalEntryWindow.h"

#define LOCTEXT_NAMESPACE "FGameReverseTimeEditorModule"
DEFINE_LOG_CATEGORY(GameReverseTimeEditor);
void FGameReverseTimeEditorModule::StartupModule()
{
	this->PluginCommands = MakeShareable(new FUICommandList);

	//注册window窗口
	this->ExternalEntryWindow = MakeShareable(new FExternalEntryWindow);
	this->ExternalEntryWindow->Register(this->PluginCommands);
}

void FGameReverseTimeEditorModule::ShutdownModule()
{
	this->ExternalEntryWindow->Shutdown();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameReverseTimeEditorModule, GameReverseTimeEditor)