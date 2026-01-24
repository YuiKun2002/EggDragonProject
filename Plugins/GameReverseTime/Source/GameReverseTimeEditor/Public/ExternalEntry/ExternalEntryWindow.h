// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"
#include "AssetRegistry/AssetData.h"
#include "ExternalEntryWindow.generated.h"

UENUM()
enum class  EPakPlatform : uint8 {
	Windows UMETA(DisplayName = "Windows"),
	Android UMETA(DisplayName = "Android(ETC2)"),
	IOS UMETA(DisplayName = "IOS")
};

class FExternalEntryWindow
{

public:
	void Register(const TSharedPtr<class FUICommandList>& UICommandListRef);
	void Shutdown();

	void PluginButtonClicked();
	void Gen();
	void OnAssetSelected(const FPropertyChangedEvent& Property);

private:

	FString GetPlatformParam(EPakPlatform Pa);

	FString GetPlatformName(EPakPlatform Pa);

	TArray<FString> EachAllFile(FString Path);

	bool Coock(EPakPlatform Pa,bool bDebug);

	void GenPak(EPakPlatform Pl, TArray<FDirectoryPath> Paths,bool bDebug);

	void RegisterMenus();

	void Layout();

private:

	FString PakName;
	FString ExecTag;
	FString TargetPath;
	FString CurPackageName;
	TArray<FDirectoryPath> DPaths;
private:

	TSharedPtr<class FUICommandList> CommandListPtr;
	TSharedPtr<class SWindow> NewWindow;

	//生成按钮
	TSharedPtr<class SButton> Generate;
	//细节视图
	TSharedPtr<class IDetailsView> DetailView;
	//导出名称
	TSharedPtr<class SEditableTextBox> EditableTextBox;
	//mod介绍
	TSharedPtr<class SEditableTextBox> EditableDesTextBox;
	//mod版本
	TSharedPtr<class SEditableTextBox> EditableVersionTextBox;

	static UGameObjectInterfaceExport* GameObjectInterfaceExport;
};

//游戏对象接口导出
UCLASS()
class UGameObjectInterfaceExport : public UObject
{
	GENERATED_BODY()
public:
	//导出的文件目录，输出为Pak
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RelativeToGameContentDir, LongPackageName))
	FDirectoryPath AssetIncludeDirectoryPath;
	//外部资产执行入口，继承自UGameMain资产
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class UGameMain> ExportProgramInterface;
	//生成Pak的对应平台
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EPakPlatform> BuildPlatform;
	/*
		导出LuaScript目录
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bExportLuaScriptDirectory = false;
	/*
	* 每次导出前，自动进行烘焙资产
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoCook = true;
	//会显示，打包Pak后的挂载细节
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowPakCreateDetail = false;
};