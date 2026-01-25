// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/GameDataName.h"
#include "UObject/NoExportTypes.h"
#include "GameDataAsset.generated.h"

//数据资产对象【全局唯一数据表结构的项目Child】
UCLASS(BlueprintType, Blueprintable)
class GAMEREVERSETIME_API UGameDataAssetObject : public UDataAsset
{
	GENERATED_BODY()
public:
	/*
		数据资产标记的ID名称
		1.数据包内唯一
		2.如果冲突请修改，否则无法载入
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "资产ID")
	FString GameDataAssetID;
	//列表下所有的资产数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "数据集合")
	TMap<FName, TSoftObjectPtr<UDataTable>> Datas;
public:
	UFUNCTION(BlueprintPure, Category = "获取数据表")
	UDataTable* GetDataByName(FName Name);
	UDataTable* GetDataByName_Imp(GameDataName&& Name);
	//强制同步加载所有的数据表
	UFUNCTION(BlueprintCallable, Category = "数据表加载")
	void ForceLoadAllData();
	//获取已经加载的数据表格资产
	UFUNCTION(BlueprintPure, Category = "获取已加载的数据表格资产组")
	FORCEINLINE	TMap<FName, class UDataTable*> GetAllDataInstance() const { return this->DataInstance; }
	//卸载
	UFUNCTION()
	void Unload();
private:
	//已经完成加载的数据表格
	UPROPERTY()
	TMap<FName, UDataTable*> DataInstance;
};

//缓存对象的例子
/*
// DataTableAssetData<T>
// FTableRowBase
// #include "Engine/DataTable.h"
//数据表结构继承
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FGameDataAssetCacheTempStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
};

// UCardDataAssetCache
// UEquipmentDataAssetCache
//继承缓存模板
UCLASS(BlueprintType, Blueprintable)
class GAMEREVERSETIME_API UGameDataAssetCacheTemp : public UGameDataAssetCache
{
	GENERATED_BODY()
public:

	//返回对应类型的数据，如果没有初始化则自行检测并且初始化数据
	TArray<FGameDataAssetCacheTempStruct> GetDataTable()
	{
		//使用方法例子如下
		return GET_SOURCE_DATA(FGameDataAssetCacheTempStruct,
			this->TempDataTable,
			this->TempSourceDataTable,
			GameVS,
			Buff
		);

		return this->TempDataTable;
	}

	//初始化数据表的其他方法，需要自行判断是否已经初始化
	void InitDataTableOtherFunc() {

		TArray<FGameDataAssetCacheTempStruct> LoadData;
		auto TempInitData = [&](GameDataName&& Name) {
			UGameSystemFunction::GetDataTableRows(
				UGameDataSubsystem::GetGameDataSubsystemStatic(WorldObj)->GetAsset()->
				GetDataByName(GET_MOVE_CATEGORYNAME(GameVS))->GetDataByName(Name), LoadData);
			};

		TempInitData(GET_MOVE_NAME(GameVS,Buff));

	}

private:
	//原生数据表
	DataTableAssetData<FGameDataAssetCacheTempStruct> TempSourceDataTable;
	//加载的数据表缓存数据
	UPROPERTY()
	TArray<FGameDataAssetCacheTempStruct> TempDataTable;
};
*/

// #include "Core/GameDataSubsystem.h"
//缓存对象->持续到关卡被卸载
UCLASS(BlueprintType, Blueprintable)
class GAMEREVERSETIME_API UGameDataAssetCache : public UGameObject
{
	GENERATED_BODY()
public:
	//通过软类生成一个资产缓存
	UFUNCTION(BlueprintPure)
	static UGameDataAssetCache* MakeGameDataAssetCache(
		const UObject* WorldContextObject,
		TSoftClassPtr<UGameDataAssetCache> BpGameDataAssetCache
	);
public:

	/*
		BP_GetNameTag C++ 解释
		1.C++的创建使用GET_CACHE()，即可获取
		2.GET_CACHE() 唯一标识是用的{类}表示，在开发过程中就直接规避重名问题！
		3.GET_CACHE() 仅限制实现了类的数据资产调用，否则必定失败
	*/


	/*
		资产生成的名称
		1.蓝图必须实现，作用于数据系统缓存和查询的唯一标识
	*/
	UFUNCTION(BlueprintImplementableEvent)
	FName BP_GetNameTag();

	//初始化缓存
	UFUNCTION(BlueprintNativeEvent)
	void InitCache(UGameDataSubsystem* GameDataSubsystem);
	virtual void InitCache_Implementation(UGameDataSubsystem* GameDataSubsystem);

	//卸载数据
	UFUNCTION(BlueprintNativeEvent)
	void Unload();
	virtual void Unload_Implementation();

public:
#if WITH_EDITOR
	//判断这个缓存资产是否是蓝图版本【主要是在开发过程中使用，打包后不需要】
	FORCEINLINE bool bIsBlueprint() { return this->bBp; }
#endif
private:
#if WITH_EDITOR
	//是否是蓝图版本的数据缓存
	bool bBp = false;
#endif
};


