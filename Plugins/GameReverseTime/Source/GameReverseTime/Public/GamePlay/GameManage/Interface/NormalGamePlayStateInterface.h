#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NormalGamePlayStateInterface.generated.h"

/**
 * 正常游戏玩法状态接口
 * 1.常规模式游戏状态接口
 * 2.主要用于获取游戏中的缓存的数据
 * 3.例如：火苗数量，修改火苗数量
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UNormalGamePlayStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 正常游戏玩法状态接口
 * 1.常规模式游戏状态接口
 * 2.主要用于获取游戏中的缓存的数据
 * 3.例如：火苗数量，修改火苗数量
 */
class GAMEREVERSETIME_API INormalGamePlayStateInterface
{
	GENERATED_BODY()

public:
	//获取火苗数量
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NormalGamePlayStateInterface")
	int32 GetFlame();
	virtual int32 GetFlame_Implementation() { return 0; }


public:
	//设置火苗数量
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NormalGamePlayStateInterface")
	void SetFlame(int32 NewFlame);
	virtual void SetFlame_Implementation(int32 NewFlame) {}

public:
	/*
		创建火苗
		1.每个火苗的值（默认25）
		2.一共创建多少个火苗
		3.创建火苗的位置
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NormalGamePlayStateInterface")
	void CreateFlameActor(int32 FlameValue,int32 FlameCount,FVector CreateLocation);
	virtual void CreateFlameActor_Implementation(int32 FlameValue,int32 FlameCount,FVector CreateLocation) {}
};

