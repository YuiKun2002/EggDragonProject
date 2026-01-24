#include "Core/GameAssetSubsystem.h"
#include "GameReverseTime.h"
#include "Core/GameDataAsset.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Core/DataTableAssetData.h"

UGameAssetSubsystem* NGASub::Get(const UObject* WorldContextObject)
{
	return UGameAssetSubsystem::GetGameAssetSubsystemStatic(WorldContextObject);
}

UGameDataAssetObject* UGamePackDataAsset::GetDataByName(const UObject* WorldContextObject, TSoftClassPtr<UAssetCategoryName> GameDataAssetID) {
	FName CurName = TSubclassOf_StaticLoadObject<UAssetCategoryName>(GameDataAssetID, WorldContextObject)->GetCategoryName();
	return this->GetDataByName_Imp(MoveTempIfPossible(CurName));
}

UGameDataAssetObject* UGamePackDataAsset::GetDataByName_Imp(FName&& GameDataAssetID) {
	UGameDataAssetObject** Asset = this->GameDataAssets.Find(GameDataAssetID);
	return Asset ? *Asset : nullptr;
}

void UGameAssetSubsystem::NewGamePackDataAsset(FSoftObjectPath GameDataPath, const FGamePackDataAssetLoaded& Callback) {
	TSoftObjectPtr<UGamePackDataAsset> GameDataAsset(GameDataPath);
	UGamePackDataAsset* NewGameDataAsset = GameDataAsset.LoadSynchronous();
	if (IsValid(NewGameDataAsset))
	{
		if (this->AssetsIns.Contains(NewGameDataAsset->PackageID))
		{
#if !UE_BUILD_SHIPPING
			UE_LOG(GameReverseTime, Log, TEXT("NewGamePack：数据包{%s},已经加载完成！不再执行委托！"), *NewGameDataAsset->PackageID);
#endif
			return;
		}

		//如果当前处于加载状态，则添加到队列
		if (!this->ExecuteAssetLoadList.IsEmpty())
		{
			if (!this->AddAssetLoadWaitArea.Contains(NewGameDataAsset->PackageID))
			{
				FAsyncLoadStruct NewAsyncLoadStruct;
				NewAsyncLoadStruct.GamePackDataAssetLoaded = Callback;
				NewAsyncLoadStruct.GamePackDataAsset = NewGameDataAsset;
				this->AddAssetLoadWaitArea.Emplace(NewGameDataAsset->PackageID, MoveTempIfPossible(NewAsyncLoadStruct));
#if !UE_BUILD_SHIPPING
				UE_LOG(GameReverseTime, Log, TEXT("NewGamePack：数据包{%s},已经添加到待加载区域！"), *NewGameDataAsset->PackageID);
#endif
			}
			else {
#if !UE_BUILD_SHIPPING
				UE_LOG(GameReverseTime, Error, TEXT("NewGamePack：数据包{%s},已经被添加到待加载区域！请勿重复添加！"), *NewGameDataAsset->PackageID);
#endif
			}
		}
		else {
			FAsyncLoadStruct NewAsyncLoadStruct;
			NewAsyncLoadStruct.StreamableHandle = this->GetStreamableManager().RequestAsyncLoad(
				NewGameDataAsset->GameDataList,
				FStreamableDelegate::CreateUObject(
					this, &UGameAssetSubsystem::AsyncLoadComplete
				)
			);
			NewAsyncLoadStruct.GamePackDataAssetLoaded = Callback;
			NewAsyncLoadStruct.GamePackDataAsset = NewGameDataAsset;
			this->ExecuteAssetLoadList.Emplace(NewGameDataAsset->PackageID, MoveTempIfPossible(NewAsyncLoadStruct));
#if !UE_BUILD_SHIPPING
			UE_LOG(GameReverseTime, Log, TEXT("NewGamePack：数据包{%s},正在进行加载！"), *NewGameDataAsset->PackageID);
#endif
		}

		return;
	}

#if !UE_BUILD_SHIPPING
	UE_LOG(GameReverseTime, Error, TEXT("NewGamePack：构造数据资产失败！路径：%s"), *GameDataPath.ToString());
#endif
}

void UGameAssetSubsystem::AddGamePackDataAsset(UGamePackDataAsset* AddedData) {
	if (IsValid(AddedData))
	{
		//如果此包已经被导入
		if (this->AssetsIns.Contains(AddedData->PackageID))
		{
#if !UE_BUILD_SHIPPING
			UE_LOG(GameReverseTime, Error, TEXT("AsyncLoad添加数据包失败!PackageID{%s}已经存在！"), *AddedData->PackageID);
#endif
		}
		else {
			this->AssetsIns.Emplace(AddedData->PackageID, AddedData);
			return;
		}
	}

#if !UE_BUILD_SHIPPING
	UE_LOG(GameReverseTime, Error, TEXT("AsyncLoad当前数据包异常!此对象无效"));
#endif
}

void UGameAssetSubsystem::AsyncLoadComplete() {

	//完成这一批的加载
	for (auto It = this->ExecuteAssetLoadList.CreateIterator(); It; ++It)
	{
		FAsyncLoadStruct& CurStr = It->Value;
		TArray<UObject*> CurObjects;
		CurStr.StreamableHandle->GetLoadedAssets(CurObjects);
		CurStr.GamePackDataAsset->GetAllGameDataAssets().Empty();
		for (UObject* ObjectInstance : CurObjects)
		{
			UGameDataAssetObject* Ins = Cast<UGameDataAssetObject>(ObjectInstance);
			if (IsValid(Ins))
			{
				if (CurStr.GamePackDataAsset->GetAllGameDataAssets().Contains(*Ins->GameDataAssetID))
				{
#if !UE_BUILD_SHIPPING
					UE_LOG(GameReverseTime, Error, TEXT("AsyncLoad：数据资产添加失败!GameDataAssetID{%s}已经存在！"), *Ins->GameDataAssetID);
#endif
				}
				else {
					CurStr.GamePackDataAsset->GetAllGameDataAssets().Add(*Ins->GameDataAssetID, Ins);
				}

			}

		}

		//添加到列表
		this->AddGamePackDataAsset(CurStr.GamePackDataAsset);

#if !UE_BUILD_SHIPPING
		UE_LOG(GameReverseTime, Log, TEXT("AsyncLoad：数据包{%s},已经完成加载！"), *CurStr.GamePackDataAsset->PackageID);
#endif

		//通知
		if (CurStr.GamePackDataAssetLoaded.IsBound())
		{
#if !UE_BUILD_SHIPPING
			UE_LOG(GameReverseTime, Log, TEXT("AsyncLoad：数据包{%s},执行函数委托！"), *CurStr.GamePackDataAsset->PackageID);
#endif
			CurStr.GamePackDataAssetLoaded.Execute(CurStr.GamePackDataAsset);
#if !UE_BUILD_SHIPPING
			UE_LOG(GameReverseTime, Log, TEXT("AsyncLoad：数据包{%s},委托执行完成！"), *CurStr.GamePackDataAsset->PackageID);
#endif
		}
	}
	this->ExecuteAssetLoadList.Empty();


	//判断等待区域,如果等待区域无加载请求，则表示完成加载
	if (this->AddAssetLoadWaitArea.IsEmpty())
	{
#if !UE_BUILD_SHIPPING
		UE_LOG(GameReverseTime, Log, TEXT("AsyncLoad：所有的数据包已经全部加载完毕！"));
#endif
		return;
	}

	//将等待资源转移
	this->ExecuteAssetLoadList = MoveTempIfPossible(this->AddAssetLoadWaitArea);
	this->AddAssetLoadWaitArea.Empty();

	for (auto It = this->ExecuteAssetLoadList.CreateIterator(); It; ++It)
	{
		FAsyncLoadStruct& CurStr = It->Value;
		CurStr.StreamableHandle = this->GetStreamableManager().RequestAsyncLoad(
			CurStr.GamePackDataAsset->GameDataList,
			FStreamableDelegate::CreateUObject(
				this, &UGameAssetSubsystem::AsyncLoadComplete
			)
		);
#if !UE_BUILD_SHIPPING
		UE_LOG(GameReverseTime, Log, TEXT("AsyncLoad：数据包{%s},正在进行加载！"), *CurStr.GamePackDataAsset->PackageID);
#endif
	}
}

UGamePackDataAsset* UGameAssetSubsystem::GetMainGameAsset(const FString& PackageID)
{
	UGamePackDataAsset** DataAssetObj = this->AssetsIns.Find(PackageID);
	if (DataAssetObj && IsValid(*DataAssetObj))
	{
		return (*DataAssetObj);
	}
#if !UE_BUILD_SHIPPING
	UE_LOG(GameReverseTime, Error, TEXT("数据资产失败！PackageID无效：%s"), *PackageID);
#endif
	return nullptr;
}

FStreamableManager& UGameAssetSubsystem::GetStreamableManager()
{
	return UAssetManager::GetStreamableManager();
}

UGameAssetSubsystem* UGameAssetSubsystem::GetGameAssetSubsystemStatic(const UObject* WorldContextObject)
{
	if (IsValid(WorldContextObject) && UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UGameAssetSubsystem>();
	}
	return nullptr;
}

bool UGameAssetSubsystem::CheckPackage(TSharedPtr<class DataTablePackageObject>& PackageObject)
{
	if (PackageObject.IsValid())
	{
		//查询表格
		TMap<FString, TSharedPtr<class DataTablePackageObject>>* Maps = this->Packages.Find(PackageObject->GetPackageID());
		if (Maps)
		{
			return Maps->Contains(PackageObject->GetGlobalPackageID());
		}
	}
	return false;
}

void UGameAssetSubsystem::AddPackage(TSharedPtr<class DataTablePackageObject>& PackageObject)
{
	if (PackageObject.IsValid())
	{
		//查询表格
		TMap<FString, TSharedPtr<class DataTablePackageObject>>* Maps = this->Packages.Find(PackageObject->GetPackageID());
		if (Maps)
		{
			Maps->Emplace(PackageObject->GetGlobalPackageID(), PackageObject);
		}
		else {
			TMap<FString, TSharedPtr<class DataTablePackageObject>> NewMap;
			NewMap.Emplace(PackageObject->GetGlobalPackageID(), PackageObject);
			this->Packages.Emplace(PackageObject->GetPackageID(), NewMap);
		}
	}
}

TSharedPtr<class DataTablePackageObject> UGameAssetSubsystem::GetPackage(const FString& PackageID, GameDataCategoryName&& DataCategoryName, GameDataName&& Name) {
	//查询表格
	TMap<FString, TSharedPtr<class DataTablePackageObject>>* Maps = this->Packages.Find(PackageID);
	if (Maps)
	{
		TSharedPtr<class DataTablePackageObject>* TargetData =
			Maps->Find(ConvertGlobalPackageID(PackageID, DataCategoryName.ToString(), Name.ToString()));
		if (TargetData)
		{
			//返回表格
			return *TargetData;
		}
	}
	return nullptr;
}