#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameInstanceInterface.generated.h"

/**
 * 逆转时光运行时全局游戏实例
 * 1.可以访问一部分在运行时中缓存的数据
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UGameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 逆转时光运行时全局游戏实例
 * 1.可以访问一部分在运行时中缓存的数据
 */
class GAMEREVERSETIME_API IGameInstanceInterface
{
	GENERATED_BODY()

public:

	//获取运行时游戏全局配置数据对象
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameInstanceInterface")
	class UObject* GetGameGlobalConfigDataObject();
	virtual class UObject* GetGameGlobalConfigDataObject_Implementation() { return nullptr; }

};
