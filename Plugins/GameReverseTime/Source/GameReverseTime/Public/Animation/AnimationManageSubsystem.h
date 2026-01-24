#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Animation/AnimationStruct.h"
#include "AnimationManageSubsystem.generated.h"

/**
 * 动画资源管理器子系统
 */
UCLASS()
class GAMEREVERSETIME_API UAnimationManageSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()


public:

	//加载动画组合
	void RequestLoadAnimation(class UAnimationComponent* AnimComp);
	void RequestLoadAnimationWidget(class UAnimationWidget* AnimComp);

	//添加动画
	FORCEINLINE void AddAnimation(const FSoftObjectPath& AnimPath, TObjectPtr<class UPaperFlipbook> AnimPtr) {
		this->Animations.Emplace(AnimPath, AnimPtr);
	}

	//获取动画
	FORCEINLINE TObjectPtr<class UPaperFlipbook> GetAnimation(const FSoftObjectPath& AnimPath) {
		return *this->Animations.Find(AnimPath);
	}

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FSoftObjectPath, TObjectPtr<class UPaperFlipbook>> Animations;

};
