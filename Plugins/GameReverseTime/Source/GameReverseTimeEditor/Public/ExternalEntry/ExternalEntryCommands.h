// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ExternalEntryStyle.h"

class FExternalEntryCommands : public TCommands<FExternalEntryCommands>
{
public:

	FExternalEntryCommands()
		: TCommands<FExternalEntryCommands>(TEXT("GameReverseTime"), NSLOCTEXT("Contexts", "GameReverseTime", "GameReverseTime Plugin"), NAME_None, FExternalEntryStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
