#pragma once
#include "Core/GameObject.h"
#include "Styling/SlateTypes.h"
#include "LoginOperateButtonGameObject.generated.h"

/*
	操作按钮委托(底层就是函数指针)
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOperateButtonClickedEvent);

/*
	操作按钮
*/
UCLASS()
class GAMEREVERSETIME_API ULoginOperateButtonGameObject : public UGameObject
{
	GENERATED_BODY()

public:

	//界面大小-最好在100-150之间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoginOperateButtonGameObject")
	FVector2D Size = FVector2D(100.f);

	//按钮样式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LoginOperateButtonGameObject")
	FButtonStyle Style;

	//执行委托-点击按钮后执行什么？
	UPROPERTY(BlueprintAssignable, Category = "LoginOperateButtonGameObject | Event")
	FOnOperateButtonClickedEvent OnButtonClicked;
};