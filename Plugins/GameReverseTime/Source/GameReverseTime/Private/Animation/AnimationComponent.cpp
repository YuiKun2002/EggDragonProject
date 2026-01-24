#include "Animation/AnimationComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Animation/AnimationManageSubsystem.h"

UAnimationComponent::UAnimationComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

#if WITH_EDITOR
	this->PlayAnimName = FName("AnimName");
#else 
	this->PlayAnimName = FName();
#endif
}


void UAnimationComponent::AnimationBeginPlay_Implementation()
{
	if (this->OnAnimationLoadedDelegate.IsBound())
	{
		this->OnAnimationLoadedDelegate.Broadcast();
	}
	else {
		//获取当前动画是否有效
		if (this->GetFlipbook() == nullptr)
		{
			this->PlayAnimation(this->Animations.FirstPlayAnimationName, this->Animations.bAnimationLoop);
		}
	}
}

void UAnimationComponent::BeginPlay()
{

#if WITH_EDITOR
	this->bPreview = false;
#endif

	Super::BeginPlay();

	if (!IsValid(this->AnimManage))
	{
		// 初始化动画管理系统
		this->AnimManage = this->GetWorld()->GetSubsystem<UAnimationManageSubsystem>();
		this->AnimLoadedStreamableHandle.Reset();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s动画管理系统初始化失败！"), *FString(__FUNCTION__));
		this->SetTickableWhenPaused(true);
		return;
	}

	//如果有动画
	if (this->Animations.Animations.Num())
	{
		//请求并加载动画
		this->AnimManage->RequestLoadAnimation(this);
	}
	else {
		this->OnAnimationLoaded();
	}
}

void UAnimationComponent::OnAnimationLoaded()
{
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

	this->AnimationBeginPlay();
}
void UAnimationComponent::OnInternalAnimRequestLoaded()
{
	for (const auto& AnimHD : this->InternalAnimLoadedStreamableHandles)
	{
		if (AnimHD.Value->HasLoadCompleted())
		{
			//动画加载完成
			FAnimation* Anim = this->Animations.Animations.Find(AnimHD.Key);
			if (Anim)
			{
				TObjectPtr<UPaperFlipbook> AnimObject = Cast<UPaperFlipbook>(Anim->Anim.ResolveObject());
				Anim->AnimInstance = AnimObject;
				this->AnimManage->AddAnimation(Anim->Anim, AnimObject);
				this->PlayAnimation(AnimHD.Key, Anim->bLoop);
			}
			this->InternalAnimLoadedStreamableHandles.Remove(AnimHD.Key);
			return;
		}
	}
}

void UAnimationComponent::OnAnimationEnd()
{

	//动画触发器
	FAnimation* AnimConfig = this->Animations.Animations.Find(this->PlayAnimName);
	if (AnimConfig)
	{
		if (AnimConfig->bAnimTrigger)
		{
#if WITH_EDITOR
			if (this->CurLevelTick != ELevelTick::LEVELTICK_ViewportsOnly)
			{
				if (this->OnAnimationTriggerEndDelegate.IsBound()) {
					this->OnAnimationTriggerEndDelegate.Broadcast(this->PlayAnimName);
				}
			}
#else
			if (this->OnAnimationTriggerEndDelegate.IsBound()) {
				this->OnAnimationTriggerEndDelegate.Broadcast(this->PlayAnimName);
			}
#endif
		}
	}

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

#if WITH_EDITOR
	//在编辑器中，不能运行代理函数
	if (this->OnAnimationEndDelegate.IsBound()) {
		if (this->CurLevelTick != ELevelTick::LEVELTICK_ViewportsOnly)
		{
			this->OnAnimationEndDelegate.Broadcast(this->PlayAnimName);
		}
	}
#else
	if (this->OnAnimationEndDelegate.IsBound()) {
		this->OnAnimationEndDelegate.Broadcast(this->PlayAnimName);
	}
#endif

	if (!this->IsLooping())
	{
		this->bPlaying = 0U;
		this->SetTickableWhenPaused(true);
	}

	this->CalculateFrame(0);
}

void UAnimationComponent::CalculateFrame(float NewPosition)
{
	float OldPosition = this->AccumulatedTime;
	this->AccumulatedTime = NewPosition;
	if (OldPosition != AccumulatedTime)
	{
		const int32 LastCachedFrame = CachedFrameIndex;
		CachedFrameIndex = (SourceFlipbook != nullptr) ? SourceFlipbook->GetKeyFrameIndexAtTime(AccumulatedTime) : INDEX_NONE;

		if (CachedFrameIndex != LastCachedFrame)
		{
			MarkRenderDynamicDataDirty();
		}
	}
}

void UAnimationComponent::SetAnimationProperty(const bool& bLoop)
{
	if ((this->SourceFlipbook != nullptr))
	{
		this->bPlaying = 1U;
		this->AnimLength = this->SourceFlipbook->GetTotalDuration();
		if (this->AnimLength == 0.f)
		{
			this->SetTickableWhenPaused(true);
			return;
		}
		this->SetLooping(bLoop);
		this->CalculateFrame(0);
		this->SetTickableWhenPaused(false);
	}
	else {
		this->SetTickableWhenPaused(true);
	}
}

bool UAnimationComponent::SetPlayAnimationAsset(const FName& AnimName)
{
	//查询是否有预设动画
	FAnimation* Anim = this->Animations.Animations.Find(AnimName);
	if (Anim)
	{
		//获取并初始化动画
		if (Anim->AnimInstance)
		{
			this->SetFlipbook(Anim->AnimInstance);
		}
		else {
			//先查找当前动画是否在内存中存在！
			UPaperFlipbook* AnimIns = Cast<UPaperFlipbook>(Anim->Anim.ResolveObject());
			if (IsValid(AnimIns))
			{
				Anim->AnimInstance = AnimIns;
				this->SetFlipbook(Anim->AnimInstance);
			}
			else {
				//内存中不存在，但是曾经加载过!
				//初始化动画缓存
				Anim->AnimInstance = this->AnimManage->GetAnimation(Anim->Anim);
				if (IsValid(Anim->AnimInstance))
				{
					this->SetFlipbook(Anim->AnimInstance);
				}
				else {
					return false;
				}
			}
		}

		this->PlayAnimName = AnimName;

#if WITH_EDITOR
		if (this->CurLevelTick != ELevelTick::LEVELTICK_ViewportsOnly)
		{
			//在编辑器中，不能运行代理函数
			if (this->OnAnimationTriggerBeginDelegate.IsBound()) {
				this->OnAnimationTriggerBeginDelegate.Broadcast(
					this->PlayAnimName,
					Anim->AnimInstance->GetTotalDuration() * this->PlayRate
				);
			}
		}
#else
		if (this->OnAnimationTriggerBeginDelegate.IsBound()) {
			this->OnAnimationTriggerBeginDelegate.Broadcast(
				this->PlayAnimName,
				Anim->AnimInstance->GetTotalDuration() * this->PlayRate
			);
		}
#endif
		return true;
	}
	else {

		UE_LOG(LogTemp, Warning, TEXT("%s当前动画资产不存在!请检查动画配置!"), *FString(__FUNCTION__));

		this->SetFlipbook(nullptr);
	}
	return false;
}

#if WITH_EDITOR
void UAnimationComponent::PreviewTick(const float& DeltaTime)
{
	this->PreviewSetAnim(this->Animations.FirstPlayAnimationName, this->Animations.bAnimationLoop);

	if (this->GetFlipbook() != nullptr)
	{
		this->UpdateAnimation(DeltaTime);
	}

	this->SetLooping(this->Animations.bAnimationLoop);
}

void UAnimationComponent::PreviewSetAnim(FName AnimName, bool bLoop)
{
	if (!this->PlayAnimName.IsEqual(AnimName))
	{
		this->PlayAnimName = AnimName;
		FAnimation* Anim = this->Animations.Animations.Find(AnimName);
		if (Anim)
		{
			if (Anim->Anim.ResolveObject() == nullptr) {
				this->PlayAnimationPtr(Cast<UPaperFlipbook>(Anim->Anim.TryLoad()), bLoop);
			}
			else {
				this->PlayAnimationPtr(Cast<UPaperFlipbook>(Anim->Anim.ResolveObject()), bLoop);
			}
		}
		else {
			this->SetFlipbook(nullptr);
		}
	}
}
#endif

void UAnimationComponent::UpdateAnimation(const float& DeltaTime)
{

	if (this->bPlaying == 0U)
	{
		return;
	}

	float NewPosition = 0.f;

	//倒放
	if (this->bReversePlayback > 0U)
	{
		NewPosition = this->AccumulatedTime + DeltaTime * (-this->PlayRate);
		if (NewPosition < 0.0f)
		{
			this->OnAnimationEnd();
			return;
		}
	}
	else
	{
		//正向播放
		NewPosition = this->AccumulatedTime + DeltaTime * this->PlayRate;
		if (NewPosition > this->AnimLength)
		{
			this->OnAnimationEnd();
			return;
		}
	}

	this->CalculateFrame(NewPosition);
}


// Called every frame
void UAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

#if WITH_EDITOR
	this->CurLevelTick = TickType;
	if (this->bPreview)
	{
		this->PreviewTick(DeltaTime);
		return;
	}
	else {
		if (TickType == ELevelTick::LEVELTICK_ViewportsOnly)
		{
			if (this->GetFlipbook() != nullptr)
			{
				this->SetFlipbook(nullptr);
			}
			return;
		}
	}
#endif

	this->UpdateAnimation(DeltaTime);
}

void UAnimationComponent::PlayAnimation(FName AnimName, bool bLoop)
{
#if WITH_EDITOR
	if (this->bPreview)
	{
		UE_LOG(LogTemp, Error, TEXT("%s你在预览动画的情况下使用了PlayAnimation()！请在打包之前尽快修改！"), *FString(__FUNCTION__));
		return;
	}
	else {
		if (!IsValid(this->AnimManage))
		{
			UE_LOG(LogTemp, Error, TEXT("%s无法播放动画！因为动画管理器无效！请在打包之前尽快修改！"), *FString(__FUNCTION__));
			return;
		}
	}
#endif
	if (this->SetPlayAnimationAsset(AnimName))
	{
		//清理序列动画
		this->PlayAnimSequence.Empty();
		this->SetAnimationProperty(bLoop);
	}
}

void UAnimationComponent::PlayNewAnimation(FSoftObjectPath AnimPath, FName AnimName, bool bLoop)
{
	//查询动画是否已经被缓存
	FAnimation* Anim = this->Animations.Animations.Find(AnimName);
	if (Anim)
	{
		if (Anim->AnimInstance)
		{
			this->PlayAnimationPtr(Anim->AnimInstance, bLoop);
		}
		else {
			this->PlayAnimation(AnimName, bLoop);
		}
	}
	else {
		//如果请求新的动画时，则进行动画资源加载！
		if (this->InternalAnimLoadedStreamableHandles.Contains(AnimName) == false)
		{

			//将动画资源缓存
			FAnimation NewAnim;
			NewAnim.Anim = AnimPath;
			NewAnim.AnimInstance = nullptr;
			NewAnim.bLoop = bLoop;
			this->Animations.Animations.Emplace(AnimName, NewAnim);

			//如果动画没有被加载过！进行异步加载！
			FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
			TSharedPtr<FStreamableHandle> TempAnimLoadedHD = StreamableManager.RequestAsyncLoad(
				MoveTempIfPossible(AnimPath),
				MoveTempIfPossible(FStreamableDelegate::CreateUObject(this,
					&UAnimationComponent::OnInternalAnimRequestLoaded))
			);
			this->InternalAnimLoadedStreamableHandles.Emplace(AnimName, TempAnimLoadedHD);
		}
	}
}

void UAnimationComponent::PlayAnimationSequence(TArray<FName> Anims)
{

	this->PlayAnimSequence.Empty();

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

void UAnimationComponent::PlayAnimationPtr(UPaperFlipbook* Flipbook, bool bLoop)
{
	this->PlayAnimSequence.Empty();
	this->SetFlipbook(Flipbook);
	this->SetAnimationProperty(bLoop);
}

