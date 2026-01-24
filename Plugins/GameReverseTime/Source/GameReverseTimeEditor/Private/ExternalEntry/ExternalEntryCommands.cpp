// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExternalEntry/ExternalEntryCommands.h"

#define LOCTEXT_NAMESPACE "FGameReverseTimeEditorModule"

void FExternalEntryCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction,
		"GameReverseTime", "GameReverseTime",
		EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
