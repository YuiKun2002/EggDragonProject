#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamePlay/CoreType/GameGrid.h"
#include "GamePlay/CoreType/CoreGame2D.h"
#include "GameRoleActor.generated.h"

/**
 * 游戏角色接口
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UGameRoleActorInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 * 游戏角色接口
 */
class GAMEREVERSETIME_API IGameRoleActorInterface
{
	GENERATED_BODY()

public:

	/*
	* 准备阶段
	* 1.通常用于初始化数据
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameRoleActor | Event")
	void GameBegin();
	virtual void GameBegin_Implementation() {}

	//当角色被攻击时触发
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameRoleActor | Event")
	void OnAttacked();
	virtual void OnAttacked_Implementation() {}

	//当角色触发死亡时触发
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameRoleActor | Event")
	void OnDeath();
	virtual void OnDeath_Implementation() {}

};

/*
	在关卡中最基本的类型
	1.场景里如果有任何想要展示的角色最好都继承此类型【统一】
	2.支持多人，请勾选复制属性
*/
UCLASS()
class GAMEREVERSETIME_API AGameRoleActor : public AActor, public IGameRoleActorInterface
{
	GENERATED_BODY()

public:

	AGameRoleActor();

public:

	//获取格子点位
	UFUNCTION(BlueprintPure, Category = "GameRoleActor | Grid")
	FORCEINLINE FGridPoint GetGridPoint() const { return GRIDPOINT(this->GetActorLocation()); }

	//获取格子点位坐标
	UFUNCTION(BlueprintPure, Category = "GameRoleActor | Grid")
	FVector2D GetGridPointLocation() const;

	//获取角色的坐标
	UFUNCTION(BlueprintPure, Category = "GameRoleActor | Role")
	FORCEINLINE FVector2D GetGameRoleActorLocation() const { return FVETOV2D(this->GetActorLocation()); }

	//获取游戏管理器
	UFUNCTION(BlueprintPure, Category = "GameRoleActor | GameManage")
	AActor* GetGameManageActor();

};
