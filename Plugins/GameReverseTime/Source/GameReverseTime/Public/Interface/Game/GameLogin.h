#pragma once

#include "CoreMinimal.h"
#include "Interface/GameInterface.h"
#include "Interface/Game/GameLoginInterface/GameLoginInterface.h"
#include "GameLogin.generated.h"

extern GAMEREVERSETIME_API const FString GAMELOGIN;
/**
 * 登录界面
 * 1.当登录界面完成初始化后，执行此入口
 * 2.直接覆盖GameLoginInit函数即可
 */
UCLASS()
class GAMEREVERSETIME_API UGameLogin : public UGameMain
{
	GENERATED_BODY()

public:
	virtual bool Main_Implementation(class UObject* CallObject) override;
	//获取执行标签
	virtual FString GetExecTag() const { return GAMELOGIN; }
protected:
	//登录初始化
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameLogin")
	void GameLoginInit();
	//获取登录接口
	UFUNCTION(BlueprintPure, Category = "GameLogin")
	void GetLoginInterface(TScriptInterface<class IGameLoginInterface>& OutInterface) { OutInterface = this->LoginInterface; }
private:
	UPROPERTY()
	TScriptInterface<class IGameLoginInterface> LoginInterface;
};
