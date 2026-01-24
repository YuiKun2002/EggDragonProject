#include "Core/GameDataSubsystem.h"
#include "GameReverseTime.h"
#include "Kismet/GameplayStatics.h"
#include "Core/DataTableAssetData.h"
#include <Kismet/KismetSystemLibrary.h>

UDataTable* __GetSourceDataTable_(const UObject* WorldObj, FName&& Name1, FName&& Name2, const FString& PackageID) {
	return UGameDataSubsystem::GetGameDataSubsystemStatic(WorldObj)->
		GetGamePackDataAsset(PackageID)->GetDataByName_Imp(MoveTemp(Name1))->GetDataByName_Imp(MoveTemp(Name2));
}

void UGameDataSubsystem::RegistPackageToCoreGameData(TSharedPtr<class DataTablePackageObject>& PackageObject)
{
	if (PackageObject.IsValid())
	{
		//移除当前库的引用
		TMap<FString, TSharedPtr<class DataTablePackageObject>>* Maps = this->Packages.Find(PackageObject->GetPackageID());
		if (Maps)
		{
			Maps->Remove(PackageObject->GetGlobalPackageID());
		}

		NGASub::Get(this)->AddPackage(PackageObject);
	}
}

bool UGameDataSubsystem::CheckRegistPackage(TSharedPtr<class DataTablePackageObject>& PackageObject)
{
	return NGASub::Get(this)->CheckPackage(PackageObject);
}

TSharedPtr<DataTablePackageObject> UGameDataSubsystem::GetPackage(const FString& PackageID, const GameDataCategoryName& DataCategoryName, const GameDataName& Name)
{

	FName CategoryName = DataCategoryName;
	FName TabName = Name;

	//查询核心数据
	TSharedPtr<class DataTablePackageObject> TargetData = NGASub::Get(this)->GetPackage(PackageID, MoveTempIfPossible(CategoryName), MoveTempIfPossible(TabName));
	if (TargetData.IsValid())
	{
		return TargetData;
	}

	//查询游戏数据表格
	TMap<FString, TSharedPtr<class DataTablePackageObject>>* Maps = this->Packages.Find(PackageID);
	if (Maps)
	{
		TSharedPtr<class DataTablePackageObject>* FindTargetData =
			Maps->Find(ConvertGlobalPackageID(PackageID, DataCategoryName.ToString(), Name.ToString()));
		if (FindTargetData)
		{
			//返回表格
			return *FindTargetData;
		}
	}

	//新增数据
	UGamePackDataAsset* Asset = this->GetGamePackDataAsset(PackageID);
	if (Asset)
	{
		UGameDataAssetObject* DataCategoryNameObj = Asset->GetDataByName_Imp(MoveTempIfPossible(CategoryName));
		if (DataCategoryNameObj)
		{
			//生成新的表格
			UDataTable* TabIns = DataCategoryNameObj->GetDataByName_Imp(MoveTempIfPossible(TabName));
			TargetData = MakeShareable(
				new DataTablePackageObject(
					PackageID,
					DataCategoryName.ToString(),
					Name.ToString(),
					TabIns
				)
			);

			if (Maps)
			{
				Maps->Emplace(TargetData->GetGlobalPackageID(), TargetData);
			}
			else {
				TMap<FString, TSharedPtr<class DataTablePackageObject>> NewPak;
				NewPak.Emplace(TargetData->GetGlobalPackageID(), TargetData);
				this->Packages.Emplace(PackageID, NewPak);
			}
			return TargetData;
		}
	}

	//无效表格
	return TSharedPtr<DataTablePackageObject>(nullptr);
}

void UGameDataSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	//执行数据加载
	this->ForceExecuteDataLoadDelegate();
}

UGameDataSubsystem* UGameDataSubsystem::GetGameDataSubsystemStatic(const UObject* WorldContextObject)
{
	if (IsValid(WorldContextObject) && WorldContextObject->GetWorld())
	{
		return WorldContextObject->GetWorld()->GetSubsystem<UGameDataSubsystem>();
	}
	return nullptr;
}

void UGameDataSubsystem::ForceExecuteDataLoadDelegate()
{
	//获取资产子系统
	UGameAssetSubsystem* AssetSub = NGASub::Get(this->GetWorld());
	if (IsValid(AssetSub) && AssetSub->OnGameDataSubsystemDataload.IsBound())
	{
		AssetSub->OnGameDataSubsystemDataload.Broadcast(this);
	}
}

UGamePackDataAsset* UGameDataSubsystem::GetGamePackDataAsset(const FString& PackageID)
{
	if (IsValid(this->GamePackDataAsset) && this->GamePackDataAsset->PackageID.Equals(PackageID))
	{
		return this->GamePackDataAsset;
	}

	UGameAssetSubsystem* AssetSub = NGASub::Get(this);
	this->GamePackDataAsset = AssetSub->GetMainGameAsset(PackageID);
	return this->GamePackDataAsset;
}

void UGameDataSubsystem::AddGameDataAssetCache(FName Name, UGameDataAssetCache* CacheIns)
{
	this->AddGameDataAssetCache_Imp(MoveTemp(Name), CacheIns);
}

void UGameDataSubsystem::AddGameDataAssetCache_Imp(GameDataCategoryName&& Name, UGameDataAssetCache* CacheIns)
{
	UGameDataAssetCache** TargetIns = this->GameDataAssetCache.Find(Name);
	if (!TargetIns)
	{
		this->GameDataAssetCache.Emplace(Name, CacheIns);
	}
}

UGameDataAssetCache* UGameDataSubsystem::GetGameDataAssetCache(FName Name)
{
	return this->GetGameDataAssetCache_Imp(MoveTemp(Name));
}

UDataTable* UGameDataSubsystem::GetGameDataAssetDataTableByName(const FString& PackageID, TSubclassOf<UAssetCategoryName> DataCategoryName, TSubclassOf<UAssetCategoryName> Name)
{
	UGameObjectLib::IsValidGameObject(this,DataCategoryName.GetDefaultObject());
	UGameObjectLib::IsValidGameObject(this,Name.GetDefaultObject());
	FName A = DataCategoryName.GetDefaultObject()->GetCategoryName();
	FName B = Name.GetDefaultObject()->GetCategoryName();
	return this->GetGameDataAssetDataTableByName_Imp(
		PackageID,
		MoveTemp(A),
		MoveTemp(B)
	);
}

UDataTable* UGameDataSubsystem::GetGameDataAssetDataTableByName_Imp(const FString& PackageID, GameDataCategoryName&& DataCategoryName, GameDataName&& Name)
{
	//获取资产中的缓存
	UGameDataAssetObject* Cache = this->GetGamePackDataAsset(PackageID)->GetDataByName_Imp(
		MoveTemp(DataCategoryName)
	);

	if (IsValid(Cache))
	{
		return (Cache)->GetDataByName_Imp(MoveTemp(Name));
	}

	return nullptr;
}

UGameDataAssetCache* UGameDataSubsystem::GetGameDataAssetCache_Imp(GameDataCategoryName&& Name)
{
	UGameDataAssetCache** TargetIns = this->GameDataAssetCache.Find(Name);
	if (TargetIns && UGameObjectLib::IsValidGameObject(this, *TargetIns))
	{
		return *TargetIns;
	}
	return nullptr;
}

void UGameDataSubsystem::GetAllGameDataAssetCache(TArray<UGameDataAssetCache*>& OutDataAssetCache) {
	this->GameDataAssetCache.GenerateValueArray(OutDataAssetCache);
}

void UGameDataSubsystem::Deinitialize()
{
	Super::Deinitialize();

	//获取资产子系统
	UGameAssetSubsystem* AssetSub = NGASub::Get(this);
	if (IsValid(AssetSub) && AssetSub->OnGameDataSubsystemDataUnload.IsBound())
	{
		AssetSub->OnGameDataSubsystemDataUnload.Broadcast(this);
	}

	for (auto& Data : this->GameDataAssetCache)
	{
		if (IsValid(Data.Value))
		{
			Data.Value->Unload();
			Data.Value = nullptr;
		}
	}
	this->GameDataAssetCache.Empty();
	this->Packages.Empty();
}

void UGameDataSubsystem::PrintCacheLog(const FString& PackageID) {

	if (IsValid(this->GetGamePackDataAsset(PackageID)))
	{
		UE_LOG(GameReverseTime, Log, TEXT("当前数据包{%s}，正在打印已经被加载的资产对象"), *PackageID);
		for (const auto& DataInstance : this->GamePackDataAsset->GetAllGameDataAssets())
		{
			UE_LOG(GameReverseTime, Log, TEXT("数据资产对象名称：%s，GameDataAssetObject：%p；------------------------"),
				*DataInstance.Key.ToString(),
				DataInstance.Value);
			for (auto ObjectDataInstance : DataInstance.Value->GetAllDataInstance())
			{
				if (ObjectDataInstance.Value == nullptr)
				{
					UE_LOG(GameReverseTime, Log, TEXT("名称：%s，DataTable：nullptr；异常"),
						*ObjectDataInstance.Key.ToString()
					);
				}
				else {
					UE_LOG(GameReverseTime, Log, TEXT("名称：%s，DataTable：%p；"),
						*ObjectDataInstance.Key.ToString(),
						ObjectDataInstance.Value);
				}
			}
		}
	}
	else {
		UE_LOG(GameReverseTime, Log, TEXT("GamePackDataAsset：未加载！,请检查包名{%s}是否正确！"), *PackageID);
	}

	for (auto CacheIns : this->GameDataAssetCache)
	{
		UE_LOG(GameReverseTime, Log, TEXT("全局游戏资产缓存对象名称：%s，GameDataAssetCache：%p；"),
			*CacheIns.Key.ToString(), CacheIns.Value
		);
	}
}


