#pragma once

#include "CoreMinimal.h"
#include "Core/GameObject.h"
#include "GameInterface.generated.h"

//游戏接口
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UGameInterface : public UInterface
{
	GENERATED_BODY()
};

//游戏接口
class GAMEREVERSETIME_API IGameInterface
{
	GENERATED_BODY()

public:

	//游戏加载函数，被游戏加载到列表时，立刻触发
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameInterface")
	void GameLoad(class UGameAssetSubsystem* GameAssetSubsystem);
	virtual void GameLoad_Implementation(class UGameAssetSubsystem* GameAssetSubsystem) {}

	//游戏卸载函数，被游戏卸载时，立刻触发
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameInterface")
	void GameUnLoad(class UGameAssetSubsystem* GameAssetSubsystem);
	virtual void GameUnLoad_Implementation(class UGameAssetSubsystem* GameAssetSubsystem) {}

	//主要函数，执行入口
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameInterface")
	bool Main(class UObject* CallObject);
	virtual bool Main_Implementation(class UObject* CallObject) { return true; }
};

/**
 * 游戏main类
 */
UCLASS()
class GAMEREVERSETIME_API UGameMain : public UGameObject, public IGameInterface
{
	GENERATED_BODY()
public:
	//游戏加载
	virtual void GameLoad_Implementation(class UGameAssetSubsystem* GameAssetSubsystem) override;
	//程序执行
	virtual bool Main_Implementation(class UObject* CallObject) override;
	//获取执行标签
	virtual FString GetExecTag() const { return this->CustomBP_GeneratedExecTag; }
	//重写获取逻辑
	virtual UWorld* GetWorld() const override;
private:
	//蓝图自定义生成执行标签-当父类为GameMain时生效
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameMain", meta = (AllowPrivateAccess = "true"))
	FString CustomBP_GeneratedExecTag;
};
