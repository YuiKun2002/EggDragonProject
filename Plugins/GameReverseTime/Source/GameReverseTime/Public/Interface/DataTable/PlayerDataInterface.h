//  

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerDataInterface.generated.h"

/**
 * 玩家存档数据数据接口
 * 1.操作玩家存档核心接口
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UPlayerDataInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 * 玩家存档数据数据接口
 * 1.操作玩家存档核心接口
 */
class GAMEREVERSETIME_API IPlayerDataInterface
{
	GENERATED_BODY()

public:

	/*
		发送物品到背包
		1.物品所处的包ID
		2.包ID下物品的唯一ID
		3.物品的数量
		4.物品的等级
		5.发送成功后是否触发IO保存
		6.发送成功后触发获取物品的提示

		发送成功返回 true
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PlayerData")
	bool SendItem(const FString& PackageID, const FString& ItemGlobalID, int32 ItemCount, int32 ItemLevel, bool bSave = true, bool bShowTip = false);
	virtual bool SendItem_Implementation(const FString& PackageID, const FString& ItemGlobalID, int32 ItemCount, int32 ItemLevel, bool bSave, bool bShowTip) { return false; }

	/*
		查询物品数量[字符串查询速度比动态ID查询慢很多]
		1.物品所处的包ID
		2.包ID下物品的唯一ID

		查询成功返回-对应数量
		查询失败返回 -1
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PlayerData")
	int32 FindItem(const FString& PackageID, const FString& ItemGlobalID, int32 ItemLevel);
	virtual int32 FindItem_Implementation(const FString& PackageID, const FString& ItemGlobalID, int32 ItemLevel) { return -1; }

	/*
		彻底删除物品[字符串查询速度比动态ID查询慢很多]
		1.物品所处的包ID
		2.包ID下物品的唯一ID
		3.删除数量
		4.删除等级
		直接删除这个物品，包括对应等级
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PlayerData")
	bool RemoveItem(const FString& PackageID, const FString& ItemGlobalID, int32 ItemCount, int32 ItemLevel);
	virtual bool RemoveItem_Implementation(const FString& PackageID, const FString& ItemGlobalID, int32 ItemCount, int32 ItemLevel) { return false; }
};
