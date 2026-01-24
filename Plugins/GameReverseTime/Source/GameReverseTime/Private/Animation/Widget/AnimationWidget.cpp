#include "Animation/Widget/AnimationWidget.h"
#include "PaperSprite.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Animation/AnimationManageSubsystem.h"

bool UAnimationWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (!IsValid(this->AnimManage) && IsValid(this->GetWorld()))
	{
		// 初始化动画管理系统
		this->AnimManage = this->GetWorld()->GetSubsystem<UAnimationManageSubsystem>();
		this->AnimLoadedStreamableHandle.Reset();
		//重新配置动画实例
		for (auto AnimIt = this->Animations.Animations.CreateIterator(); AnimIt; ++AnimIt)
		{
			AnimIt->Value.AnimInstance = nullptr;
		}

		if (!IsValid(this->AnimManage))
		{
#if WITH_EDITOR
			if (this->SourceFlipbook == nullptr)
			{
				this->PlayAnimationWidget(
					this->Animations.FirstPlayAnimationName,
					this->Animations.bAnimationLoop
				);
			}
#endif
			return true;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s动画管理系统初始化失败！"), *FString(__FUNCTION__));
		return true;
	}

	//如果有动画
	if (this->Animations.Animations.Num())
	{
		//请求并加载动画
		this->AnimManage->RequestLoadAnimationWidget(this);
	}
	else {
		this->OnAnimationLoaded();
	}

	return true;
}

void UAnimationWidget::OnAnimationLoaded() {

	//判断是否触发加载
	if (this->AnimLoadedStreamableHandle.IsValid())
	{
		TArray<FSoftObjectPath> AnimPaths;
		this->AnimLoadedStreamableHandle->GetRequestedAssets(AnimPaths, false);

		for (auto AnimIt = AnimPaths.CreateIterator(); AnimIt; ++AnimIt)
		{
			TObjectPtr<UPaperFlipbook> AnimObject = Cast<UPaperFlipbook>(AnimIt->ResolveObject());
			this->AnimManage->AddAnimation(*AnimIt, AnimObject);
		}

		this->AnimLoadedStreamableHandle.Reset();
	}

	if (this->OnAnimationLoadedDelegate.IsBound())
	{
		this->OnAnimationLoadedDelegate.Broadcast();
	}
	else {
		//获取当前动画是否有效
		if (this->SourceFlipbook == nullptr)
		{
			this->PlayAnimationWidget(this->Animations.FirstPlayAnimationName, this->Animations.bAnimationLoop);
		}
	}
}

void UAnimationWidget::PlayAnimationWidget(FName AnimName, bool bLoop) {
	if (this->SetPlayAnimationAsset(AnimName))
	{
		//清理序列动画
		this->PlayAnimSequence.Empty();
		this->SetAnimationProperty(bLoop);
	}
}

void UAnimationWidget::PlayAnimationPtr(class UPaperFlipbook* Flipbook, bool bLoop) {
	this->PlayAnimSequence.Empty();
	this->SourceFlipbook = Flipbook;
	this->SetAnimationProperty(bLoop);
	this->PlayFromStart();
}

void UAnimationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//播放动画
	if (this->SourceFlipbook != nullptr && this->bAnimPlay)
	{
		if (this->AnimTime > this->AnimLength) {

			//如果有队列动画
			FName NextAnimName;
			if (this->PlayAnimSequence.Dequeue(NextAnimName))
			{
				if (this->SetPlayAnimationAsset(NextAnimName))
				{
					this->SetAnimationProperty(true);
					return;
				}

				//如果资产设置失败，则清理队列动画
				this->PlayAnimSequence.Empty();
			}

			if (this->OnAnimationEndDelegate.IsBound())
			{
				this->OnAnimationEndDelegate.Broadcast(this->PlayAnimName);
			}

			if (this->bAnimLoop)
			{
				this->AnimTime = 0.f;
			}
			else {
				this->bAnimPlay = false;
				return;
			}
		}
		this->AnimView->SetBrushResourceObject(this->SourceFlipbook->GetSpriteAtTime(this->AnimTime));
		this->AnimTime += InDeltaTime * this->PlayRate;
	}
}

void UAnimationWidget::SetAnimationProperty(const bool& bLoop) {
	if ((this->SourceFlipbook != nullptr))
	{
		this->AnimLength = this->SourceFlipbook->GetTotalDuration();
		if (this->AnimLength == 0.f)
		{
			return;
		}
		this->bAnimPlay = true;
		this->bAnimLoop = bLoop;
		//设置大小
		UPaperSprite* Sprite = this->SourceFlipbook->GetSpriteAtTime(0.f);
		if (Sprite->GetBakedTexture())
		{
			this->AnimSize->SetWidthOverride(Sprite->GetBakedTexture()->GetSizeX());
			this->AnimSize->SetHeightOverride(Sprite->GetBakedTexture()->GetSizeY());
		}
	}
}

bool UAnimationWidget::SetPlayAnimationAsset(const FName& AnimName) {
	//查询是否有预设动画
	FAnimation* Anim = this->Animations.Animations.Find(AnimName);
	if (Anim)
	{
		//获取并初始化动画
		if (Anim->AnimInstance)
		{
			this->SourceFlipbook = Anim->AnimInstance;
		}
		else {
			if (IsValid(this->AnimManage))
			{
				//先查找当前动画是否在内存中存在！
				UPaperFlipbook* AnimIns = Cast<UPaperFlipbook>(Anim->Anim.ResolveObject());
				if (IsValid(AnimIns))
				{
					Anim->AnimInstance = AnimIns;

					this->SourceFlipbook = AnimIns;
				}
				else {

					//内存中不存在，但是曾经加载过!
					//初始化动画缓存
					Anim->AnimInstance = this->AnimManage->GetAnimation(Anim->Anim);
					this->SourceFlipbook = Anim->AnimInstance;
				}
			}
			else {
				Anim->AnimInstance = Cast<UPaperFlipbook>(Anim->Anim.TryLoad());
				if (IsValid(Anim->AnimInstance))
				{
					this->SourceFlipbook = Anim->AnimInstance;
#if WITH_EDITOR
					this->AnimView->SetBrushResourceObject(this->SourceFlipbook->GetSpriteAtTime(0.f));
#endif
				}
			}
		}
		return true;
	}
	else {
		this->SourceFlipbook = nullptr;
	}

	return false;
}

void UAnimationWidget::PlayAnimationSequence(TArray<FName> Anims) {
	for (auto AnimIt = Anims.CreateConstIterator(); AnimIt; ++AnimIt)
	{
		this->PlayAnimSequence.Enqueue(*AnimIt);
	}

	//播放动画
	FName NewAnim;
	if (this->PlayAnimSequence.Dequeue(NewAnim))
	{
		if (this->SetPlayAnimationAsset(NewAnim))
		{
			this->SetAnimationProperty(true);
		}
	}
	else {
		this->PlayAnimSequence.Empty();
	}
}