// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "UObject/Interface.h"
#include "Core/GameDataName.h"
#include "Core/DataTableAssetData.h"//表格数据处理类型
#include "Engine/StreamableManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameAssetSubsystem.generated.h"

class UGameAssetSubsystem;
/**
 * 游戏资产子系统
 */
namespace NGASub {
	GAMEREVERSETIME_API UGameAssetSubsystem* Get(const UObject* WorldContextObject);
}

/**
 * 游戏资产包
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEREVERSETIME_API UGamePackDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	/*
		数据包ID名称
		1.全局唯一
		2.如果冲突请修改，否则无法载入
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "游戏数据")
	FString PackageID;

	//游戏数据表列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "游戏数据")
	TArray<FSoftObjectPath> GameDataList;


public:

	//获取游戏数据资产
	UFUNCTION(BlueprintPure, Category = "游戏数据", meta = (WorldContext = "WorldContextObject"))
	class UGameDataAssetObject* GetDataByName(const UObject* WorldContextObject, TSoftClassPtr<UAssetCategoryName> GameDataAssetID);
	class UGameDataAssetObject* GetDataByName_Imp(FName&& GameDataAssetID);

	//获取全部的数据资产
	FORCEINLINE	TMap<FName, class UGameDataAssetObject*>& GetAllGameDataAssets() { return this->GameDataAssets; }

private:
	//已经被加载的数据
	UPROPERTY()
	TMap<FName, class UGameDataAssetObject*> GameDataAssets;
};





/**
 * 游戏资产包加载完成的委托
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FGamePackDataAssetLoaded, class UGamePackDataAsset*, PackDataAsset);


//异步加载结构
USTRUCT()
struct GAMEREVERSETIME_API FAsyncLoadStruct {

	GENERATED_USTRUCT_BODY()

	//当前句柄
	TSharedPtr<FStreamableHandle> StreamableHandle;

	//当前被加载的资产
	UPROPERTY()
	class UGamePackDataAsset* GamePackDataAsset = nullptr;

	//当前加载请求的回调
	FGamePackDataAssetLoaded GamePackDataAssetLoaded;
};

/*
	数据子系统被加载时
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameDataSubsystemDataload, class UGameDataSubsystem*, DataSubsystem);
/*
	数据子系统被卸载时
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameDataSubsystemDataUnload, class UGameDataSubsystem*, DataSubsystem);

/**
 * 游戏资产子系统-全局存在
 */
UCLASS()
class GAMEREVERSETIME_API UGameAssetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//数据子系统被加载时
	UPROPERTY(BlueprintAssignable)
	FGameDataSubsystemDataload OnGameDataSubsystemDataload;
	//数据子系统被卸载时
	UPROPERTY(BlueprintAssignable)
	FGameDataSubsystemDataUnload OnGameDataSubsystemDataUnload;

public:

	virtual void Deinitialize() { this->Packages.Empty(); this->DynamicID = 0; }

	//构造游戏数据包资产对象
	UFUNCTION(BlueprintCallable, Category = "资产")
	void NewGamePackDataAsset(FSoftObjectPath GameDataPath, const FGamePackDataAssetLoaded& Callback);

	/*
	* 检测数据包
	*/
	bool CheckPackage(TSharedPtr<class DataTablePackageObject>& PackageObject);

	/*
	新增数据包
	1.添加新的数据包到缓存
	*/
	void AddPackage(TSharedPtr<class DataTablePackageObject>& PackageObject);

	/*
	获取动态ID
	*/
	FORCEINLINE int32 GetDynamicID() const { return this->DynamicID; }

	/*
	更新动态ID
	*/
	FORCEINLINE void UpdateDynamicID() { ++this->DynamicID; }

	/*
	设置动态ID
	*/
	FORCEINLINE void SetDynamicID(int32 NewDynamicID) { this->DynamicID = NewDynamicID; }

public:
	//获取主要的游戏资产
	UFUNCTION(BlueprintPure, Category = "主要资产")
	class UGamePackDataAsset* GetMainGameAsset(const FString& PackageID);
	//获取子系统
	UFUNCTION(BlueprintPure, Category = "游戏资产子系统", meta = (WorldContext = "WorldContextObject"))
	static UGameAssetSubsystem* GetGameAssetSubsystemStatic(const UObject* WorldContextObject);
	/*
	获取数据包
	1.数据包里面直接提供各种对数据表的获取函数
	2.直接通过唯一ID获取对应的数据表格
	*/
	TSharedPtr<class DataTablePackageObject> GetPackage(const FString& PackageID, GameDataCategoryName&& DataCategoryName, GameDataName&& Name);
protected:
	//异步加载完成
	void AsyncLoadComplete();
	//新增游戏数据包
	void AddGamePackDataAsset(class UGamePackDataAsset* AddedData);
private:
	//获取加载管理器
	FStreamableManager& GetStreamableManager();
private:

	//物品ID索引
	UPROPERTY()
	int32 DynamicID = 0;

	//资产表格
	UPROPERTY()
	TMap<FString, class UGamePackDataAsset*> AssetsIns;

	//资产加载队列[等待加载区域]
	UPROPERTY()
	TMap<FString, FAsyncLoadStruct> AddAssetLoadWaitArea;
	//资产加载队列[正在加载的区域]
	UPROPERTY()
	TMap<FString, FAsyncLoadStruct> ExecuteAssetLoadList;
	//数据包[最底层的数据表]-直到游戏结束-清理
	TMap<FString, TMap<FString, TSharedPtr<class DataTablePackageObject>>> Packages;
};
