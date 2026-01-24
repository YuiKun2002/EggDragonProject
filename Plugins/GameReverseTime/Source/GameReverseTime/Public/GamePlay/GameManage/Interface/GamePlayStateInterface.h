#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GamePlayStateInterface.generated.h"

/**
 * 默认游戏状态接口
 * 1.管理整局游戏状态：开始，结束等
 * 2.可以获取游戏玩法管理器Actor
 * 3.主要用于获取特殊组件
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UGamePlayStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 默认游戏状态接口
 * 1.管理整局游戏状态：开始，结束等
 * 2.可以获取游戏玩法管理器Actor
 * 3.主要用于获取特殊组件
 */
class GAMEREVERSETIME_API IGamePlayStateInterface
{
	GENERATED_BODY()

public:

	//获取游戏管理-默认C++实现
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GamePlayStateInterface")
	class AActor* GetGameManage();
	virtual class AActor* GetGameManage_Implementation() { return nullptr; }

	/*
		获取游戏地图数据-默认C++实现-[仅存在服务器]
		1.如果客户端要使用地图数据，请自行加载
		2.服务器中的地图数据用于做数据校验
		3.此数据仅是游戏最初始的数据，如果因为关卡内格子数据发生变化，此位置不会同步
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GamePlayStateInterface")
	class UMapDataStructAsset* GetGameMapData();
	virtual class UMapDataStructAsset* GetGameMapData_Implementation() { return nullptr; }

	//游戏是否开始-默认C++实现
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GamePlayStateInterface")
	bool IsGameStart();
	virtual bool IsGameStart_Implementation() { return false; }

	//游戏是否结束-默认C++实现
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GamePlayStateInterface")
	bool IsGameOver();
	virtual bool IsGameOver_Implementation() { return false; }
};
