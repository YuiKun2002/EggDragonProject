#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameManageInterface.generated.h"

/**
 *游戏管理器接口
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UGameManageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *游戏管理器接口
 */
class GAMEREVERSETIME_API IGameManageInterface
{
	GENERATED_BODY()

public:

	//获取游戏管理
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameManageInterface")
	class UActorComponent* GetGameComponentByTag(FName Tag);
	virtual class UActorComponent* GetGameComponentByTag_Implementation(FName Tag) { return nullptr; }

};
