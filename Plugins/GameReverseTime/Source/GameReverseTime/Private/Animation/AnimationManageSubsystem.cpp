#include "Animation/AnimationManageSubsystem.h"
#include "Animation/Widget/AnimationWidget.h"
#include "Paper2D/Classes/PaperFlipbook.h"
#include "Animation/AnimationComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

void UAnimationManageSubsystem::RequestLoadAnimation(UAnimationComponent* AnimComp)
{
	//如果当前动画组件已经请求过一次后，禁止再次请求
	if (AnimComp->AnimLoadedStreamableHandle.IsValid())
	{
		return;
	}

	TArray<FAnimation> Anim;
	AnimComp->Animations.Animations.GenerateValueArray(Anim);

	TArray<FSoftObjectPath> LoadPath;
	for (auto AnimIt = Anim.CreateIterator(); AnimIt; ++AnimIt)
	{
		TObjectPtr<UPaperFlipbook>* Ptr = this->Animations.Find(AnimIt->Anim);
		if (Ptr == nullptr)
		{
			//收集还未加载的资产
			LoadPath.Emplace(AnimIt->Anim);
		}
	}

	//如果存在需要加载的动画
	if (LoadPath.Num())
	{

		//触发加载任务
		// 假设在GameInstance中
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		AnimComp->AnimLoadedStreamableHandle = StreamableManager.RequestAsyncLoad(
			MoveTempIfPossible(LoadPath),
			MoveTempIfPossible(FStreamableDelegate::CreateUObject(AnimComp, &UAnimationComponent::OnAnimationLoaded))
		);
	}
	else {
		//直接通过动画组件进行动画播放
		AnimComp->OnAnimationLoaded();
	}
}

void UAnimationManageSubsystem::RequestLoadAnimationWidget(UAnimationWidget* AnimComp)
{
	//如果当前动画组件已经请求过一次后，禁止再次请求
	if (AnimComp->AnimLoadedStreamableHandle.IsValid())
	{
		return;
	}

	TArray<FAnimation> Anim;
	AnimComp->Animations.Animations.GenerateValueArray(Anim);

	TArray<FSoftObjectPath> LoadPath;
	for (auto AnimIt = Anim.CreateIterator(); AnimIt; ++AnimIt)
	{
		TObjectPtr<UPaperFlipbook>* Ptr = this->Animations.Find(AnimIt->Anim);
		if (Ptr == nullptr)
		{
			//收集还未加载的资产
			LoadPath.Emplace(AnimIt->Anim);
		}
	}

	//如果存在需要加载的动画
	if (LoadPath.Num())
	{

		//触发加载任务
		// 假设在GameInstance中
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		AnimComp->AnimLoadedStreamableHandle = StreamableManager.RequestAsyncLoad(
			MoveTempIfPossible(LoadPath),
			MoveTempIfPossible(FStreamableDelegate::CreateUObject(AnimComp, &UAnimationWidget::OnAnimationLoaded))
		);
	}
	else {
		//直接通过动画组件进行动画播放
		AnimComp->OnAnimationLoaded();
	}
}
