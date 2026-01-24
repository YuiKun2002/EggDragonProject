#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameGlobalConfigDataInterface.generated.h"

/**
 * 运行时全局游戏配置数据
 * 1.配置数据无法持久化，一旦退出游戏数据清除！
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UGameGlobalConfigDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 运行时全局游戏配置数据
 * 1.配置数据无法持久化，一旦退出游戏数据清除！
 */
class GAMEREVERSETIME_API IGameGlobalConfigDataInterface
{
	GENERATED_BODY()

public:

	/*
		添加玩家注册执行对象
		1.当有新的角色存档被创建时，会执行GameObject对象
		2.可以利用这个对象在创建新角色时操作角色存档
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameGlobalConfigDataInterface")
	void AddPlayerRegistExecuteObject(const TSoftClassPtr<class UGameObject>& ObjectPath);
	virtual void AddPlayerRegistExecuteObject_Implementation(const TSoftClassPtr<class UGameObject>& ObjectPath) {};

};