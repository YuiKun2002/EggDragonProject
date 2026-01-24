#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataInterface.generated.h"

/**
 * 物品数据接口
 * 1.将数据添加到游戏的核心接口
 * 2.例如：添加到游戏内置的[装备][卡片][材料]数据中
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UItemDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 物品数据接口
 * 1.将数据添加到游戏的核心接口
 * 2.例如：添加到游戏内置的[装备][卡片][材料]数据中
 */
class GAMEREVERSETIME_API IItemDataInterface
{
	GENERATED_BODY()

public:

	//注册数据表包对象
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ItemData")
	bool RegistDataTablePackage(class UDataTablePackageObject* PackageObject);
	virtual bool RegistDataTablePackage_Implementation(class UDataTablePackageObject* PackageObject) { return false; }

	/*
		通过全局唯一名称查询ID
		1.GlobalPackageItemName 全局唯一ID号[例如:GR.AA.小笼包]
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ItemData")
	int32 GetID(const FString& GlobalPackageItemName);
	virtual int32 GetID_Implementation(const FString& GlobalPackageItemName) { return this->GetItemID(GlobalPackageItemName); }

public:
	/*
	通过ID获取物品
	1.ItemID 物品的动态ID号
	2.ItemCategoryType 道具类型分类类型[装备类型][卡片类型][材料类型]
	*/
	virtual bool GetItem(int32 ItemID, uint8& ItemCategoryType, void*& OutData) = 0;

	/*
		通过名称查询ID
		1.GlobalPackageItemName 全局唯一ID号[例如:GR.AA.小笼包]
	*/
	virtual int32 GetItemID(const FString& GlobalPackageItemName) { return -1; }
};

