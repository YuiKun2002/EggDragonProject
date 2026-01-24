#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/DataTable/Item/Card.h"
#include "CardComponentInterface.generated.h"

/**
 * 卡片组件接口
 * 1.管理卡片的一切状态
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UCardComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 卡片组件接口
 * 1.管理卡片的一切状态
 * 2.卡片接口的一切函数调用都需要在服务器上执行！除非你只做单人
 * 3.TagName:CardComponentInterface
 */
class GAMEREVERSETIME_API ICardComponentInterface
{
	GENERATED_BODY()

public:

	//获取卡片数据缓存-[必须在服务器执行]
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CardComponentInterface")
	bool GetCardDataCache(int32 DynamicID, FCardTableRow& CardData);
	virtual bool GetCardDataCache_Implementation(int32 DynamicID, FCardTableRow& CardData) { return false; }

	//加载卡片的能力数据表
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CardComponentInterface")
	bool LoadCardAbilityTables(const TSet<TSoftObjectPtr<class UObjectAbilityDetailDataAsset>>& Abilitys);
	virtual bool LoadCardAbilityTables_Implementation(const TSet<TSoftObjectPtr<class UObjectAbilityDetailDataAsset>>& Abilitys) { return false; }

	//获取卡片的能力数据表
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CardComponentInterface")
	class UObjectAbilityDetailDataAsset* GetCardAbilityTable(const TSet<TSoftObjectPtr<class UObjectAbilityDetailDataAsset>>& Abilitys, EObjectAbility ObjectAbility, const FString& CustomObjectAbilityName);
	virtual class UObjectAbilityDetailDataAsset* GetCardAbilityTable_Implementation(const TSet<TSoftObjectPtr<class UObjectAbilityDetailDataAsset>>& Abilitys, EObjectAbility ObjectAbility, const FString& CustomObjectAbilityName) { return nullptr; }
};