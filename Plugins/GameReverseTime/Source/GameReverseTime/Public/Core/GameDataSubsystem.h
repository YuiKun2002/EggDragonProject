#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "Core/GameObject.h"//游戏数据对象
#include "Core/GameDataName.h"//修改全局资产名称
#include "Core/GameDataAsset.h"//全局数据表资产
#include "Core/GameAssetSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameDataSubsystem.generated.h"



/**
 * 游戏数据子系统
 */
UCLASS()
class GAMEREVERSETIME_API UGameDataSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	friend class UGameDataAsset;
	friend class UGameDataStaticObject;

public:

	//获取数据子系统
	UFUNCTION(BlueprintPure, Category = "游戏数据子系统", meta = (WorldContext = "WorldContextObject"))
	static UGameDataSubsystem* GetGameDataSubsystemStatic(const UObject* WorldContextObject);

	//强制执行一次数据加载委托
	UFUNCTION(BlueprintCallable, Category = "游戏数据子系统 | 数据加载")
	void ForceExecuteDataLoadDelegate();

	//获取全局唯一资产，可以直接获取对应数据表【但是数据表结构需要自己转换，数据缓存需要自己处理】
	UFUNCTION(BlueprintPure, Category = "游戏数据子系统 | 获取资产")
	UGamePackDataAsset* GetGamePackDataAsset(const FString& PackageID);

	//添加缓存对象【关卡被卸载时，自动清理缓存，数据缓存由子系统管理】
	UFUNCTION(BlueprintCallable, Category = "游戏数据子系统 | 缓存对象")
	void AddGameDataAssetCache(FName Name, UGameDataAssetCache* CacheIns);
	void AddGameDataAssetCache_Imp(GameDataCategoryName&& Name, UGameDataAssetCache* CacheIns);
	//获取缓存对象【关卡被卸载时，自动清理缓存，数据缓存由子系统管理】
	UFUNCTION(BlueprintPure, Category = "游戏数据子系统 | 缓存对象")
	UGameDataAssetCache* GetGameDataAssetCache(FName Name);
	UGameDataAssetCache* GetGameDataAssetCache_Imp(GameDataCategoryName&& Name);
	//获取所有的缓存实例
	UFUNCTION(BlueprintPure, Category = "游戏数据子系统 | 缓存对象")
	void GetAllGameDataAssetCache(TArray<UGameDataAssetCache*>& OutDataAssetCache);
	//获取缓存对象的数据表【关卡被卸载时，自动清理缓存，数据缓存需要自己处理】
	UFUNCTION(BlueprintPure, Category = "游戏数据子系统 | 缓存对象")
	UDataTable* GetGameDataAssetDataTableByName(const FString& PackageID, TSubclassOf<UAssetCategoryName> DataCategoryName, TSubclassOf<UAssetCategoryName> Name);
	UDataTable* GetGameDataAssetDataTableByName_Imp(const FString& PackageID, GameDataCategoryName&& DataCategoryName, GameDataName&& Name);
	//打印数据子系统的缓存日志
	UFUNCTION(BlueprintCallable, Category = "游戏数据子系统 | 日志信息")
	void PrintCacheLog(const FString& PackageID);

	/*
		检测是否注册到核心数据
	*/
	bool CheckRegistPackage(TSharedPtr<class DataTablePackageObject>& PackageObject);

	/*
		注册数据表到核心数据表
		1.核心数据表全局初始化一次
		2.不会因为切换地图导致数据重制
	*/
	void RegistPackageToCoreGameData(TSharedPtr<class DataTablePackageObject>& PackageObject);
	/*
		获取数据包
		1.数据包里面直接提供各种对数据表的获取函数
		2.直接通过唯一ID获取对应的数据表格
	*/
	TSharedPtr<class DataTablePackageObject> GetPackage(const FString& PackageID, const GameDataCategoryName& DataCategoryName, const GameDataName& Name);
	/*
		获取数据包组合
	*/
	FORCEINLINE TMap<FString, TMap<FString, TSharedPtr<class DataTablePackageObject>>>& GetPackages() { return this->Packages; }
public:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

private:
	//游戏数据[最新的包资产引用]
	UPROPERTY()
	UGamePackDataAsset* GamePackDataAsset = nullptr;
	//资产缓存[对数据表的进一步封装操作]
	UPROPERTY()
	TMap<FName, UGameDataAssetCache*> GameDataAssetCache;
	//数据包[最底层的数据表]
	TMap<FString, TMap<FString, TSharedPtr<class DataTablePackageObject>>> Packages;
};


//获取数据表格
GAMEREVERSETIME_API UDataTable* __GetSourceDataTable_(const UObject* WorldObj, FName&& Name1, FName&& Name2, const FString& PackageID);

//获取临时数据表，建议直接使用  GET_DATA()
template<class T>
DataTableAssetData<T> _GetTempDataTableAssetData_(const UObject* WorldObj, FName&& Name1, FName&& Name2, const FString& PackageID)
{
	UDataTable* TEMPDATATABLE = UGameDataSubsystem::GetGameDataSubsystemStatic(WorldObj)->
		GetGamePackDataAsset(PackageID)->GetDataByName_Imp(MoveTemp(Name1))->GetDataByName_Imp(MoveTemp(Name2));
	return DataTableAssetData<T>(TEMPDATATABLE);
}

//获取临时数据表数组返回，建议直接使用  GET_DATA_ARRAY()	
template<class T>
TArray<T> _GetTempDataTableAssetData_Array_(const UObject* WorldObj, FName&& Name1, FName&& Name2, const FString& PackageID)
{
	UDataTable* TEMPDATATABLE = UGameDataSubsystem::GetGameDataSubsystemStatic(WorldObj)->
		GetGamePackDataAsset(PackageID)->GetDataByName_Imp(MoveTemp(Name1))->GetDataByName_Imp(MoveTemp(Name2));
	TArray<T> Temp;
	DataTableAssetData<T>(TEMPDATATABLE).GetValues(Temp);
	return Temp;
}

/*
	获取游戏数据资产缓存【模板】
	1.通常情况下你不太可能直接使用此函数
	2.直接使用此函数，会有重名风险！除非你明白自己在做什么。
*/
template<class CacheClassType>
CacheClassType* _GetGameDataAssetCache_Temp(const UObject* WorldObj, GameDataCategoryName&& Name)
{
	if (IsValid(WorldObj) && WorldObj->GetWorld())
	{
		UGameDataSubsystem* TempGameDataSubsystem = WorldObj->GetWorld()->GetSubsystem<UGameDataSubsystem>();

		if (IsValid(TempGameDataSubsystem))
		{
			UGameDataAssetCache* CurCache = TempGameDataSubsystem->GetGameDataAssetCache_Imp(
				MoveTemp(Name)
			);

			if (UGameObjectLib::IsValidGameObject(WorldObj, CurCache))
			{
				return Cast<CacheClassType>(CurCache);
			}

			CacheClassType* CurCacheType = NewGameObject<CacheClassType>(WorldObj);
			CurCacheType->InitCache(TempGameDataSubsystem);
			TempGameDataSubsystem->AddGameDataAssetCache_Imp(MoveTemp(Name), CurCacheType);

			return CurCacheType;
		}
	}
	return nullptr;
}
