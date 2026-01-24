#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CardGameRoleInterface.generated.h"

/**
 * 卡片角色接口
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UCardGameRoleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 卡片角色接口
 */
class GAMEREVERSETIME_API ICardGameRoleInterface
{
	GENERATED_BODY()

public:

	//当动态ID初始化[服务器调用]
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CardGameRoleInterface | Event")
	void OnInitDynamicID(int32 DynamicID);
	virtual void OnInitDynamicID_Implementation(int32 DynamicID) {};

	//获取动态ID
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CardGameRoleInterface | Event")
	int32 GetDynamicID();
	virtual int32 GetDynamicID_Implementation() { return -1; }

	//当卡片触发放置时触发
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CardGameRoleInterface | Event")
	void OnPlaced();
	virtual void OnPlaced_Implementation() {}

public:



};