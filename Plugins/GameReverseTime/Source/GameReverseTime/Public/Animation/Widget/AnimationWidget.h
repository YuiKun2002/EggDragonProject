#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "Blueprint/UserWidget.h"
#include "PaperFlipbookComponent.h"
#include "Engine/StreamableManager.h"
#include "Animation/AnimationStruct.h"
#include "AnimationWidget.generated.h"

/**
 * 动画界面
 * 1.可用于界面的2D序列动画
 */
UCLASS()
class GAMEREVERSETIME_API UAnimationWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class UAnimationManageSubsystem;

public:

	//动画资源加载完成
	UPROPERTY(BlueprintAssignable)
	FAnimationLoaded OnAnimationLoadedDelegate;

	//动画播放结束
	UPROPERTY(BlueprintAssignable)
	FAnimationEnd OnAnimationEndDelegate;

public:

	//播放动画
	UFUNCTION(BlueprintCallable, Category = "AnimationWidget")
	void PlayAnimationWidget(FName AnimName, bool bLoop);

	//播放动画
	UFUNCTION(BlueprintCallable)
	void PlayAnimationPtr(class UPaperFlipbook* Flipbook, bool bLoop);

	/*
		播放序列动画
		1.当整个序列动画播放完毕时才会触发动画结束
		2.最后一个动画将循环
	*/
	UFUNCTION(BlueprintCallable, Category = "AnimationWidget")
	void PlayAnimationSequence(TArray<FName> Anims);

	//设置播放速度
	UFUNCTION(BlueprintCallable, Category = "AnimationWidget")
	FORCEINLINE void SetPlayRate(float Rate) { this->PlayRate = Rate; }

	//设置循环播放
	UFUNCTION(BlueprintCallable, Category = "AnimationWidget")
	FORCEINLINE void SetLooping(bool bNewLooping) { this->bAnimLoop = bNewLooping; }

	//开始播放
	UFUNCTION(BlueprintCallable, Category = "AnimationWidget")
	FORCEINLINE void Play() { this->bAnimPlay = true; }

	//从头开始播放
	UFUNCTION(BlueprintCallable, Category = "AnimationWidget")
	FORCEINLINE void PlayFromStart() { this->AnimTime = 0.f;  this->bAnimPlay = true; }

	//停止播放
	UFUNCTION(BlueprintCallable, Category = "AnimationWidget")
	FORCEINLINE void Stop() { this->bAnimPlay = false; }

public:

	//是否正在播放
	UFUNCTION(BlueprintPure, Category = "AnimationWidget")
	FORCEINLINE bool IsPlaying() const { return this->bAnimPlay; }

	//是否循环播放
	UFUNCTION(BlueprintPure, Category = "AnimationWidget")
	FORCEINLINE bool IsLooping() const { return this->bAnimLoop; }

	//获取播放速度
	UFUNCTION(BlueprintPure, Category = "AnimationWidget")
	FORCEINLINE float GetPlayRate() const { return this->PlayRate; }

	/*
		获取动画图片组件
		1.设置颜色
		2.不透明度
	*/
	UFUNCTION(BlueprintPure, Category = "AnimationWidget")
	FORCEINLINE class UImage* GetAnimationImage() { return this->AnimView; }

public:

	virtual bool Initialize() override;

	//资源加载完毕！
	void OnAnimationLoaded();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//设置动画属性
	void SetAnimationProperty(const bool& bLoop);
	//设置动画播放资产
	bool SetPlayAnimationAsset(const FName& AnimName);
protected:

	//播放速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationWidget")
	float PlayRate = 1.f;

	//配置需要播放的动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationWidget")
	FAnimationGroup Animations;

	//动画实例
	UPROPERTY()
	TObjectPtr<UPaperFlipbook> SourceFlipbook;

	//动画管理系统
	UPROPERTY()
	class UAnimationManageSubsystem* AnimManage = nullptr;

	//动画加载完成的句柄
	TSharedPtr<FStreamableHandle> AnimLoadedStreamableHandle;
protected:
	//动画视图组件
	UPROPERTY(meta = (BindWidget))
	class UImage* AnimView = nullptr;
	//显示大小
	UPROPERTY(meta = (BindWidget))
	class USizeBox* AnimSize = nullptr;

private:
	//当前在播放的动画名称
	FName PlayAnimName;
	//是否播放
	bool bAnimPlay = true;
	//动画长度
	float AnimLength = 0.f;
	//动画时间
	float AnimTime = 0.f;
	//动画循环
	bool bAnimLoop = true;
	//动画播放序列(播放动画的顺序，如果有序列则依序播放,直到最后一个)
	TQueue<FName> PlayAnimSequence;
};
