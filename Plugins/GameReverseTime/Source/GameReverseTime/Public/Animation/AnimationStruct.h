#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AnimationStruct.generated.h"

//动画资源加载完成
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimationLoaded);

//动画播放完成
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnimationEnd, const FName&, AnimName);
/*
	动画触发器播放开始
	1.动画名称和动画播放的持续时间
	2.可以通过动画播放的持续时间来进行细节处理
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimationTriggerBegin, const FName&, AnimName,const float&, AnimDuration);
//动画触发器播放结束
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnimationTriggerEnd, const FName&, AnimName);

//动画
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FAnimation
{
	GENERATED_BODY()

	/*
		动画触发器
		1.如果勾选，当前动画如果播放完毕，立刻触发委托
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAnimTrigger = false;

	//动画引用路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath Anim;

	//动画实例
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<class UPaperFlipbook> AnimInstance;

	//动画循环
	UPROPERTY()
	bool bLoop = false;
};

//动画组合
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FAnimationGroup
{
	GENERATED_BODY()

	//配置默认动画名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "默认配置")
	FName FirstPlayAnimationName;

	//配置默认动画循环
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "默认配置")
	bool bAnimationLoop = true;

	//所有的动画组合
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画组")
	TMap<FName, FAnimation> Animations;

};


/**
 * 动画结构对象
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEREVERSETIME_API UAnimationStruct : public UObject
{
	GENERATED_BODY()
};
