#include "ExternalEntry/ExternalEntryWindow.h"
#include "ExternalEntry/ExternalEntryStyle.h"
#include "Framework/Commands/UICommandList.h"
#include "ExternalEntry/ExternalEntryCommands.h"
#include "IDetailsView.h"


#include "../../../GameReverseTime/Public/Core/GameObject.h"
#include "../../../GameReverseTime/Public/Interface/GameInterface.h"
#include "ToolMenus.h"
#include "Widgets/SWindow.h"
#include "Widgets/SBoxPanel.h"
#include "Styling/CoreStyle.h"
#include "PropertyEditorModule.h"
#include "Widgets/Input/SButton.h"
#include "Slate/WidgetTransform.h"
#include "GameReverseTimeEditor.h"
#include "Widgets/Layout/SScaleBox.h"

#include "HAL/PlatformFilemanager.h"
#include "HAL/Platform.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "PakFileUtilities.h"

UGameObjectInterfaceExport* FExternalEntryWindow::GameObjectInterfaceExport = nullptr;

void FExternalEntryWindow::Register(const TSharedPtr<class FUICommandList>& UICommandListRef) {

	FExternalEntryStyle::Initialize();
	FExternalEntryStyle::ReloadTextures();
	FExternalEntryCommands::Register();

	this->CommandListPtr = UICommandListRef;
	this->CommandListPtr->MapAction(
		FExternalEntryCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FExternalEntryWindow::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FExternalEntryWindow::RegisterMenus));
}

void FExternalEntryWindow::Shutdown() {

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FExternalEntryStyle::Shutdown();
}

void FExternalEntryWindow::RegisterMenus() {
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitToolBarButton(FExternalEntryCommands::Get().PluginAction));
				Entry.SetCommandList(this->CommandListPtr);
			}
		}
	}
}

void FExternalEntryWindow::PluginButtonClicked()
{
	if (this->NewWindow.IsValid())
	{
		this->NewWindow.Reset();
		this->Generate.Reset();
		this->DetailView.Reset();
		this->EditableTextBox.Reset();
		this->EditableDesTextBox.Reset();
		this->EditableVersionTextBox.Reset();
	}

	this->Layout();

	const FVector2D& Size = FVector2D(800, 80);

	const FVector2D& TextSize = FVector2D(150, 0);

	FSlateFontInfo CustomFont22 = FCoreStyle::Get().GetFontStyle("NormalFont");
	CustomFont22.Size = 22;
	FSlateFontInfo CustomFont24 = CustomFont22;
	CustomFont24.Size = 24;

	this->NewWindow = SNew(SWindow)
		.Title(FText::FromString(TEXT("美食大战老鼠-逆转时光-创建程序执行入口")))
		.ClientSize(FVector2D(800, 600))
		.SizingRule(ESizingRule::UserSized)
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(10)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SBox)
										.WidthOverride(TextSize.X)
										[
											SNew(STextBlock).
												Font(CustomFont22)
												.Justification(ETextJustify::Center)
												.Text(
													FText::FromString(TEXT("导出名称："))
												)
										]
								]
							+ SHorizontalBox::Slot()
								.HAlign(HAlign_Fill)
								[
									this->EditableTextBox.ToSharedRef()
								]
						]
					+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(10)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SBox)
										.WidthOverride(TextSize.X)
										[
											SNew(STextBlock).
												Font(CustomFont22)
												.Justification(ETextJustify::Center)
												.Text(
													FText::FromString(TEXT("导出介绍："))
												)
										]
								]
							+ SHorizontalBox::Slot()
								.HAlign(HAlign_Fill)
								[
									this->EditableDesTextBox.ToSharedRef()
								]
						]
					+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(10)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SBox)
										.WidthOverride(TextSize.X)
										[
											SNew(STextBlock).
												Font(CustomFont22)
												.Justification(ETextJustify::Center)
												.Text(
													FText::FromString(TEXT("导出版本："))
												)
										]
								]
							+ SHorizontalBox::Slot()
								.HAlign(HAlign_Fill)
								[
									this->EditableVersionTextBox.ToSharedRef()
								]
						]
					+ SVerticalBox::Slot()
						.FillHeight(1.f)
						.VAlign(VAlign_Fill)
						.HAlign(HAlign_Fill)
						[
							this->DetailView.ToSharedRef()
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(10)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SNew(SBox)
								.WidthOverride(Size.X)
								.HeightOverride(Size.Y)
								[
									this->Generate.ToSharedRef()
								]
						]
				]
		];


	FSlateApplication::Get().AddWindow(this->NewWindow.ToSharedRef());
}

void FExternalEntryWindow::Layout() {

	FSlateFontInfo CustomFont22 = FCoreStyle::Get().GetFontStyle("NormalFont");
	CustomFont22.Size = 22;
	FSlateFontInfo CustomFont24 = CustomFont22;
	CustomFont24.Size = 24;

	this->EditableTextBox = SNew(SEditableTextBox).
		Font(CustomFont22)
		.Justification(ETextJustify::Center)
		.Text(
			FText::FromString(TEXT("TestTemplate"))
		);

	this->EditableDesTextBox = SNew(SEditableTextBox).
		Font(CustomFont22)
		.Justification(ETextJustify::Center)
		.Text(
			FText::FromString(TEXT("内容介绍....."))
		);

	this->EditableVersionTextBox = SNew(SEditableTextBox).
		Font(CustomFont22)
		.Justification(ETextJustify::Center)
		.Text(
			FText::FromString(TEXT("1.0"))
		);

	this->Generate = SNew(SButton)
		.OnClicked_Lambda([&] {
		this->Gen();
		return FReply::Handled();
			})
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Padding(4, 2)
				[
					SNew(STextBlock)
						.Font(CustomFont22)
						.Justification(ETextJustify::Center)
						.Text(
							FText::FromString(TEXT("生成"))
						)
				]
		];
	this->Generate->SetEnabled(false);


	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked
		<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = true;

	if (this->DetailView.IsValid())
	{
		this->DetailView.Reset();
	}
	this->DetailView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	if (IsValid(FExternalEntryWindow::GameObjectInterfaceExport))
	{
		FExternalEntryWindow::GameObjectInterfaceExport->RemoveFromRoot();
		FExternalEntryWindow::GameObjectInterfaceExport = nullptr;
	}

	FExternalEntryWindow::GameObjectInterfaceExport = NewObject<UGameObjectInterfaceExport>();
	FExternalEntryWindow::GameObjectInterfaceExport->AddToRoot();

	this->DetailView->SetObject(FExternalEntryWindow::GameObjectInterfaceExport);
	this->DetailView->OnFinishedChangingProperties().AddRaw(this, &FExternalEntryWindow::OnAssetSelected);
}


// 单击资产回调
void FExternalEntryWindow::OnAssetSelected(const FPropertyChangedEvent& Property) {

	this->Generate->SetEnabled(false);

	if (FExternalEntryWindow::GameObjectInterfaceExport->ExportProgramInterface.IsValid())
	{
		if (this->Generate.IsValid())
		{
			TSoftClassPtr<class UGameMain>& Ptr = FExternalEntryWindow::GameObjectInterfaceExport->ExportProgramInterface;

			UGameMain* ObjInstance = TSubclassOf_StaticLoadObject<UGameMain>(Ptr, nullptr);
			if (IsValid(ObjInstance) && dynamic_cast<IGameInterface*>(ObjInstance))
			{
				this->DPaths = { FExternalEntryWindow::GameObjectInterfaceExport->AssetIncludeDirectoryPath };

				this->ExecTag = ObjInstance->GetExecTag();
				if (this->ExecTag.IsEmpty())
				{
					ObjInstance = nullptr;
					return;
				}

				this->PakName = this->EditableTextBox->GetText().ToString();
				if (this->PakName.IsEmpty())
				{
					ObjInstance = nullptr;
					return;
				}

				if (FExternalEntryWindow::GameObjectInterfaceExport->BuildPlatform.Num() == 0)
				{
					ObjInstance = nullptr;
					return;
				}

				this->TargetPath.Empty();
				this->TargetPath = Ptr.ToString();
				this->Generate->SetEnabled(true);

				ObjInstance = nullptr;
				return;
			}
		}
	}
}


FString FExternalEntryWindow::GetPlatformParam(EPakPlatform Pa) {
	switch (Pa)
	{
	case EPakPlatform::Windows: return TEXT("Win64");
	case EPakPlatform::Android: return TEXT("Android");
	case EPakPlatform::IOS: return TEXT("IOS");
	}
	return TEXT("");
}

FString FExternalEntryWindow::GetPlatformName(EPakPlatform Pa) {
	switch (Pa)
	{
	case EPakPlatform::Windows: return TEXT("Windows");
	case EPakPlatform::Android: return TEXT("Android");
	case EPakPlatform::IOS: return TEXT("IOS");
	}
	return TEXT("");
}

TArray<FString> FExternalEntryWindow::EachAllFile(FString Path) {

	// 用于存储文件名的数组
	TArray<FString> FileNames;

	// 获取文件管理器实例
	IFileManager& FileManager = IFileManager::Get();

	// 递归遍历目录（true表示包含子目录）
	FileManager.FindFilesRecursive(FileNames, *Path, TEXT("*"), true, false, true);

	return FileNames;
}



bool FExternalEntryWindow::Coock(EPakPlatform Pa, bool bDebug) {



	// 引擎路径与项目路径
	FString EnginePath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir());
	FString ProjectPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	FString BuildPlatform = this->GetPlatformParam(Pa);
	FString ProjectFile = ProjectPath + FString(FApp::GetProjectName()) + TEXT(".uproject");

	// 构建Cook命令
	FString CookCommand = FString::Printf(
		TEXT("%sBuild/BatchFiles/RunUAT.bat "),
		*EnginePath
	);

	FString CookCommandParam;
	if (BuildPlatform.Equals(TEXT("Android")))
	{
		CookCommandParam = FString::Printf(
			TEXT("BuildCookRun -project=\"%s\" -platform=%s -CompressedTextureFormats=ETC2 -cook"),
			*ProjectFile, *BuildPlatform
		);
	}
	else {
		CookCommandParam = FString::Printf(
			TEXT("BuildCookRun -project=\"%s\" -platform=%s -cook"),
			*ProjectFile, *BuildPlatform
		);
	}

	if (bDebug)
	{
		//批处理路径
		FString PathFilePath = FString(FPaths::ProjectDir() + this->PakName + TEXT("/Debug/") + this->GetPlatformName(Pa) + TEXT("/RunCook.bat"));
		FFileHelper::SaveStringToFile(FString(TEXT("cmd /k ") + CookCommand + CookCommandParam), *PathFilePath);
	}

	// 执行命令
	int32 Code = 0;
	FString OutDir;
	FString OutError;
	if (FPlatformProcess::ExecProcess(*CookCommand, *CookCommandParam, &Code, &OutDir, &OutError))
	{
		UE_LOG(GameReverseTimeEditor, Log, TEXT("烘焙完成!"));
		return true;
	}

	UE_LOG(GameReverseTimeEditor, Error, TEXT("烘焙失败，错误：%s"), *OutError);
	return false;
}

void FExternalEntryWindow::GenPak(EPakPlatform Pl, TArray<FDirectoryPath> Paths, bool bDebug) {

	bool bLua = FExternalEntryWindow::GameObjectInterfaceExport->bExportLuaScriptDirectory;

	FString Pa = FExternalEntryWindow::GameObjectInterfaceExport->AssetIncludeDirectoryPath.Path;
	FString Ig, PackageName;
	if (Pa.Split(TEXT("/"), &Ig, &PackageName, ESearchCase::Type::CaseSensitive, ESearchDir::Type::FromEnd))
	{
		this->CurPackageName = PackageName;
	}
	else {
		this->CurPackageName = TEXT("");
	}

	//读取对应的烘焙平台的位置
	auto GenPath = [&](EPakPlatform Plat, FDirectoryPath LocalPath)->FString {
		FString ConPath = FString::Printf(TEXT("Saved/Cooked/%s/%s/Content/"),
			*this->GetPlatformName(Plat),
			*FString(FApp::GetProjectName()));
		FString L, R;
		if (LocalPath.Path.Split(TEXT("/Game/"), &L, &R))
		{
			return FPaths::ConvertRelativePathToFull(
				FString::Printf(TEXT("%s%s%s"), *FPaths::ProjectDir(), *ConPath, *R)
			);
		}
		return TEXT("");
		};

	//输出根目录
	FString OutRootPath = FPaths::ProjectDir() + this->PakName;
	//将文件目录的位置写入的到文本文档中
	FString FilePath = FString(OutRootPath + TEXT("/DebugPakPaths.txt"));
	FString Content;
	UE_LOG(GameReverseTimeEditor, Log, TEXT("资产目录下的文件需要被打包到Pak的路径："));
	for (const auto& Path : Paths)
	{
		FString LP = GenPath(Pl, Path);
		TArray<FString> Files = this->EachAllFile(LP);
		for (const auto& F : Files)
		{
			FString L0, R0;
			if (F.Split(FPaths::GetBaseFilename(F), &L0, &R0))
			{
				FString L1, R1;
				if (L0.Split(FString::Printf(TEXT("/%s/%s/"), *this->GetPlatformName(Pl), *FString(FApp::GetProjectName())), &L1, &R1))
				{
					FString Mount = FString::Printf(TEXT("../../../%s"), *R1);
					Content.Append(FString::Printf(TEXT(" \"%s\"  \"%s\" \n"), *F, *Mount));
					UE_LOG(GameReverseTimeEditor, Log, TEXT("\"%s\" \"%s\" "), *F, *Mount);
				}

			}
		}
	}

	UE_LOG(GameReverseTimeEditor, Log, TEXT("Lua资产目录下的文件需要被打包到Pak的路径："));
	for (const auto& Path : Paths)
	{
		FString LP = FPaths::ConvertRelativePathToFull(
			FString::Printf(TEXT("%sContent/Script/%s"),*FPaths::ProjectDir(), *this->CurPackageName)
		);
		TArray<FString> Files = this->EachAllFile(LP);
		for (const auto& F : Files)
		{
			FString L0, R0;
			if (F.Split(FPaths::GetBaseFilename(F), &L0, &R0))
			{
				FString L1, R1;
				if (L0.Split(FString(FApp::GetProjectName()), &L1, &R1))
				{
					FString Mount = FString::Printf(TEXT("../../..%s"), *R1);
					Content.Append(FString::Printf(TEXT(" \"%s\"  \"%s\" \n"), *F, *Mount));
					UE_LOG(GameReverseTimeEditor, Log, TEXT("\"%s\" \"%s\" "), *F, *Mount);
				}

			}
		}
	}

	//最后保存文本文档
	if (FFileHelper::SaveStringToFile(Content, *FilePath))
	{
		//目标路径
		FString PakTargetPath = FPaths::ConvertRelativePathToFull((OutRootPath));
		//被打包的Pak路径
		FString PakPath = FString::Printf(TEXT("%s/%s_%s.pak"), *PakTargetPath, *this->PakName, *this->GetPlatformName(Pl));
		//打包内容路径
		FString PakCmdFilePath = FPaths::ConvertRelativePathToFull(FilePath);
		//项目路径
		FString ProjFullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
		//引擎路径
		FString EngineFullPath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir());
		//平台
		FString Platform = this->GetPlatformName(Pl);
		//打包命令
		FString Cmd =
			FString::Printf(TEXT(" \"%s\" -create=\"%s\" -compress -compressionformats=Zlib -projectdir=\"%s\" -enginedir=\"%s\" -platform=%s"),
				*PakPath, *PakCmdFilePath, *ProjFullPath, *EngineFullPath, *Platform);
		UE_LOG(GameReverseTimeEditor, Log, TEXT("PakCmd：%s"), *Cmd);
		ExecuteUnrealPak(*Cmd);

		if (!bDebug)
		{
			//删除
			IFileManager::Get().Delete(*PakCmdFilePath, false, true);
		}
	}
}

bool CopyDirectoryContents(const FString& SourcePath, const FString& DestPath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	bool bAllSuccess = true;

	// 复制文件
	class FFileVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		FFileVisitor(IPlatformFile& InPlatformFile, const FString& InDestRoot)
			: PlatformFile(InPlatformFile), DestRoot(InDestRoot), bAllSuccess(true) {
		}

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			FString SourceItem = FilenameOrDirectory;
			FString RelativePath = SourceItem.RightChop(SourceRoot.Len());
			FString DestItem = DestRoot + RelativePath;

			if (bIsDirectory)
			{
				// 对于目录，确保在目标位置创建
				if (!PlatformFile.DirectoryExists(*DestItem))
				{
					PlatformFile.CreateDirectory(*DestItem);
				}
			}
			else
			{
				// 对于文件，进行复制
				FString DestDir = FPaths::GetPath(DestItem);
				if (!PlatformFile.DirectoryExists(*DestDir))
				{
					PlatformFile.CreateDirectory(*DestDir);
				}

				if (!PlatformFile.CopyFile(*DestItem, *SourceItem))
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to copy: %s"), *SourceItem);
					bAllSuccess = false;
				}
			}

			return true;
		}

		IPlatformFile& PlatformFile;
		FString SourceRoot;
		FString DestRoot;
		bool bAllSuccess;
	};

	FFileVisitor Visitor(PlatformFile, DestPath);
	Visitor.SourceRoot = SourcePath;

	// 遍历并复制所有内容
	PlatformFile.IterateDirectoryRecursively(*SourcePath, Visitor);

	return Visitor.bAllSuccess;
}


bool CopyDirectory(const FString& From, const FString& To)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// 检查源目录
	if (!PlatformFile.DirectoryExists(*From))
	{
		UE_LOG(LogTemp, Error, TEXT("Source directory doesn't exist: %s"), *From);
		return false;
	}

	// 创建目标目录
	PlatformFile.CreateDirectoryTree(*To);

	// 使用迭代方式复制
	return CopyDirectoryContents(From, To);
}


void FExternalEntryWindow::Gen() {

#if WITH_EDITOR

#if PLATFORM_WINDOWS

	bool bLua = FExternalEntryWindow::GameObjectInterfaceExport->bExportLuaScriptDirectory;
	bool bCpp = !FExternalEntryWindow::GameObjectInterfaceExport->bAutoCook;
	this->DPaths = { FExternalEntryWindow::GameObjectInterfaceExport->AssetIncludeDirectoryPath };
	this->PakName = this->EditableTextBox->GetText().ToString();

	TSet<EPakPlatform> P;
	for (const auto Plat : FExternalEntryWindow::GameObjectInterfaceExport->BuildPlatform)
	{
		P.Emplace(Plat);
	}

	bool bDebug = FExternalEntryWindow::GameObjectInterfaceExport->bShowPakCreateDetail;

	bool bE = false;
	for (const auto& LP : P)
	{
		if (bCpp)
		{
			this->GenPak(LP, this->DPaths, bDebug);
			bE = true;
		}
		else {
			bE = this->Coock(LP, bDebug);
			if (bE == false)
			{
				UE_LOG(GameReverseTimeEditor, Log, TEXT("烘焙失败！"));
				return;
			}
			else
			{
				this->GenPak(LP, this->DPaths, bDebug);
			}
		}
	}


	if (bLua)
	{
		FString LuaPathDir = FString();
		UE_LOG(GameReverseTimeEditor, Log, TEXT("Lua目录："));
		for (const FDirectoryPath& LocalPath : this->DPaths)
		{
			FString L, R;
			LocalPath.Path.Split(TEXT("/Game/"), &L, &R);
			LuaPathDir = FString::Printf(TEXT("%sContent/Script/%s"), *FPaths::ProjectDir(), *R);
			UE_LOG(GameReverseTimeEditor, Log, TEXT("Lua Dir Path ：%s"), *LuaPathDir);
		}

		if (CopyDirectory(LuaPathDir, FString(FPaths::ProjectDir() + this->PakName + TEXT("/") + this->CurPackageName)))
		{
			UE_LOG(GameReverseTimeEditor, Log, TEXT("Lua目录移动完成！"));
		}
		else {
			UE_LOG(GameReverseTimeEditor, Error, TEXT("Lua目录移动失败！"));
		}

	}

	if (bE)
	{
		//创建配置文件
		FString DynamicConfigContent = TEXT("{\n");
		FString MainContent = FString::Printf(TEXT("\"ModName\" : \"%s\",\n"), *this->EditableTextBox->GetText().ToString());
		MainContent += FString::Printf(TEXT("\"ModInfor\" : \"%s\",\n"), *this->EditableDesTextBox->GetText().ToString());
		MainContent += FString::Printf(TEXT("\"ModVersion\" : \"%s\",\n"), *this->EditableVersionTextBox->GetText().ToString());
		MainContent += FString::Printf(TEXT("\"ModExecTag\" : \"%s\",\n"), *this->ExecTag);
		MainContent += FString::Printf(TEXT("\"ModExecutePath\" : \"%s\",\n"), *this->TargetPath);
		MainContent += FString::Printf(TEXT("\"ModPackageName\" : \"%s\",\n"), *this->CurPackageName);
		MainContent += FString::Printf(TEXT("\"bLuaScript\" : %s,\n"), (bLua ? *FString("true") : *FString("false")));
		MainContent += FString::Printf(TEXT("\"ModPakFile\" : {\n"));
		int32 Count = 0;
		int32 Max = P.Num();
		for (const auto& LP : P)
		{
			if (Count + 1 == Max)
			{
				MainContent += FString::Printf(TEXT("\"%s\" : \"%s_%s.pak\"\n"), *this->GetPlatformName(LP), *this->PakName, *this->GetPlatformName(LP));
			}
			else {
				MainContent += FString::Printf(TEXT("\"%s\" : \"%s_%s.pak\",\n"), *this->GetPlatformName(LP), *this->PakName, *this->GetPlatformName(LP));
			}

			Count++;
		}

		MainContent += FString::Printf(TEXT("}"));
		DynamicConfigContent += MainContent + TEXT("\n}");

		//输出根目录
		FString OutRootPath = FPaths::ProjectDir() + this->PakName + TEXT("/config.json");
		FFileHelper::SaveStringToFile(*DynamicConfigContent, *OutRootPath);

		UE_LOG(GameReverseTimeEditor, Log, TEXT("打包完成! 配置内容：%s"), *DynamicConfigContent);
	}
	else {
		UE_LOG(GameReverseTimeEditor, Log, TEXT("失败"));
	}
#else
	UE_LOG(GameReverseTimeEditor, Log, TEXT("目前只支持Window直接打包，Mac或者其他平台请直接使用Unreal提供的工具处理"));
#endif

#endif

}
