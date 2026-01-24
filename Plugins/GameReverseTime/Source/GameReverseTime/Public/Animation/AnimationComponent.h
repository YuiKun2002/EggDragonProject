#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Engine/StreamableManager.h"
#include "Animation/AnimationStruct.h"
#include "Paper2D/Classes/PaperFlipbook.h"
#include "AnimationComponent.generated.h"

/*
	2D 动画组件
	1.管理一切动画的状态
	2.动画加载全程异步处理，使用前必须确定动画已经全部加载完毕
	3.绑定OnAnimationLoadedDelegate，触发后可以安全使用
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, IsBlueprintBase = true), Blueprintable)
class GAMEREVERSETIME_API UAnimationComponent : public UPaperFlipbookComponent
{
	GENERATED_BODY()

	friend class UAnimationManageSubsystem;

public:

	/*
		动画资源加载完成
		1.如果绑定了事件，则执行事件的实现逻辑
		2.如果没有进行绑定则进行默认配置动画播放
	*/
	UPROPERTY(BlueprintAssignable)
	FAnimationLoaded OnAnimationLoadedDelegate;

	/*
		动画播放结束
		1.表示的是最后一个动画播放完毕
		2.如果你设置了一组动画序列，那么必须等序列中所有的动画播放完毕，才会触发动画结束
		3.如果你没有序列，则当前动画播放完毕立刻触发动画结束
	*/
	UPROPERTY(BlueprintAssignable)
	FAnimationEnd OnAnimationEndDelegate;

	/*
	动画触发器播放开始
	1.如果当前动画你设置了触发器
	2.只要是当前动画播放开始，都立即触发
	*/
	UPROPERTY(BlueprintAssignable)
	FAnimationTriggerBegin OnAnimationTriggerBeginDelegate;

	/*
		动画触发器播放结束
		1.如果当前动画你设置了触发器
		2.只要是当前动画播放结束，都立即触发
	*/
	UPROPERTY(BlueprintAssignable)
	FAnimationTriggerEnd OnAnimationTriggerEndDelegate;

public:

	// 动画组件默认设置
	UAnimationComponent();

	// 每帧调用
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//播放动画
	UFUNCTION(BlueprintCallable)
	void PlayAnimation(FName AnimName, bool bLoop);

	/*
		新增并播放动画
		1.新增之后会缓存到动画组内，下一次播放则可以直接使用名称进行播放
		2.如果缓存中已经存在同名称动画则直接播放缓存中的动画
		3.动画资产内部是异步加载，可能不会第一时间进行播放，动画资源大可能会有延迟
	*/
	UFUNCTION(BlueprintCallable)
	void PlayNewAnimation(FSoftObjectPath AnimPath, FName AnimName, bool bLoop);

	/*
		播放序列动画
		1.当整个序列动画播放完毕时才会触发动画结束
		2.最后一个动画将循环
	*/
	UFUNCTION(BlueprintCallable)
	void PlayAnimationSequence(TArray<FName> Anims);

	/*
		直接通过动画资源播放动画
		1.无法触发-触发器
		2.动画结束后，不会有动画名称
		3.使用时需要谨慎
	*/
	UFUNCTION(BlueprintCallable)
	void PlayAnimationPtr(class UPaperFlipbook* Flipbook, bool bLoop);

protected:

	//动画资产完成加载-游戏开始
	UFUNCTION(BlueprintNativeEvent, Category = "游戏开始")
	void AnimationBeginPlay();

protected:

	//动画资产完成加载-游戏开始
	virtual void AnimationBeginPlay_Implementation();
	//不推荐这个函数播放动画
	virtual void BeginPlay() override;
	//更新动画
	virtual void UpdateAnimation(const float& DeltaTime);
	//资源加载完毕！
	void OnAnimationLoaded();
	//内部资源请求
	void OnInternalAnimRequestLoaded();
	//动画播放完成
	void OnAnimationEnd();
	//计算动画帧
	void CalculateFrame(float NewPosition);
	//设置动画属性
	void SetAnimationProperty(const bool& bLoop);
	//设置动画播放资产
	bool SetPlayAnimationAsset(const FName& AnimName);
protected:

	//编辑器预览-仅在编辑器中
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	bool bPreview = true;

	//配置需要播放的动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
	FAnimationGroup Animations;

	//动画管理系统
	UPROPERTY()
	class UAnimationManageSubsystem* AnimManage = nullptr;

	//动画加载完成的句柄
	TSharedPtr<FStreamableHandle> AnimLoadedStreamableHandle;

	//组件内部动画加载完成的句柄[处理多请求]
	TMap<FName, TSharedPtr<FStreamableHandle>> InternalAnimLoadedStreamableHandles;

private:
	//当前在播放的动画名称
	FName PlayAnimName;
	//动画长度
	float AnimLength = 0.f;
	//动画播放序列(播放动画的顺序，如果有序列则依序播放,直到最后一个)
	TQueue<FName> PlayAnimSequence;
#if WITH_EDITOR
	ELevelTick CurLevelTick = ELevelTick::LEVELTICK_All;
	void PreviewTick(const float& DeltaTime);
	void PreviewSetAnim(FName AnimName, bool bLoop);
#endif

};
