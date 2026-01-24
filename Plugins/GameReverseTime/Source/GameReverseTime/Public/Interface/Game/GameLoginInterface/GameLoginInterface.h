#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameLoginInterface.generated.h"


UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UGameLoginInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 游戏登录接口
 */
class GAMEREVERSETIME_API IGameLoginInterface
{
	GENERATED_BODY()

public:

	/*
		额外添加一个按钮操作项目
		1.新的按钮对象
		2.添加到什么位置？ 0 , x
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameLoginInterface")
	void AddExtraOperate(class ULoginOperateButtonGameObject* NewExtraObject, int32 IndexPosition = -1);
	virtual void AddExtraOperate_Implementation(class ULoginOperateButtonGameObject* NewExtraObject, int32 IndexPosition) {}


	/*
		获取已经被添加的按钮操作项目
		2.什么位置？ 0 , x
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameLoginInterface")
	class ULoginOperateButtonGameObject* GetExtraOperate(int32 IndexPosition);
	virtual class ULoginOperateButtonGameObject* GetExtraOperate_Implementation(int32 IndexPosition) { return nullptr; }

	/*
		创建一个提示
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "CreateTipWidget"), Category = "GameLoginInterface")
	void BP_CreateTipWidget(const FString& Content);
	virtual void  BP_CreateTipWidget_Implementation(const FString& Content) {}
};