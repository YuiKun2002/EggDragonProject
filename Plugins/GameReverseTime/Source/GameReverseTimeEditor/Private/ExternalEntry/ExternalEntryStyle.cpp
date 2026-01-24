// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExternalEntry/ExternalEntryStyle.h"
#include "GameReverseTimeEditor.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir
#define IMAGE_BRUSHL(RelativePath, ...) FSlateImageBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
TSharedPtr<FSlateStyleSet> FExternalEntryStyle::StyleInstance = nullptr;

void FExternalEntryStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FExternalEntryStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FExternalEntryStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ExternalEntryStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef< FSlateStyleSet > FExternalEntryStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("ExternalEntryStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("GameReverseTime")->GetBaseDir() / TEXT("Resources"));

	Style->Set("GameReverseTime.PluginAction", new IMAGE_BRUSHL(TEXT("ExternalEntry"), Icon40x40));
	return Style;
}

void FExternalEntryStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FExternalEntryStyle::Get()
{
	return *StyleInstance;
}
