#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/NoExportTypes.h"
#include "ObjectAbilityDataAsset.generated.h"


//对象能力
UENUM(BlueprintType)
enum class EObjectAbility : uint8 {
	/*
		自定义能力
		1.自定义数据
	*/
	CustomAbility UMETA(DisplayName = "CustomAbility"),
	/*
		生成器
		1.可以生成一个Actor对象
	*/
	Generator UMETA(DisplayName = "Generator"),
	/*
		计时器
		1.可以进行时间倒计时
		2.倒计时结束后可以执行步骤
	*/
	Timer UMETA(DisplayName = "Timer"),
	/*
		火苗-生产能力相关的对象预设
		1.提供了对火苗的细节数据设置
	*/
	Flame UMETA(DisplayName = "Flame"),


	/*
		最大计数
		1.无效选项，除非有循环需求
	*/
	Max = Timer + 1U UMETA(DisplayName = "Max")
};

/*
	对象能力细节数据对象
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEREVERSETIME_API UObjectAbilityDetailDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	//能力类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ObjectAbilityDetailDataAsset")
	EObjectAbility ObjectAbility = EObjectAbility::Generator;

	/*
		自定义能力名称
		1.当 ObjectAbility 设置为 EObjectAbility::CustomAbility 时用于对能力的数据的匹配
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ObjectAbilityDetailDataAsset")
	FString CustomObjectAbilityName;

	//能力数据关联表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ObjectAbilityDetailDataAsset")
	TSoftObjectPtr<class UDataTable> AbilityDataTable;

	//数据表实例
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDataTable> AbilityDataTableIns = nullptr;

	//获取能力数据表
	UFUNCTION(BlueprintPure, Category = "ObjectAbilityDetailDataAsset")
	FORCEINLINE class UDataTable* GetAbilityDataTable() { return this->AbilityDataTableIns; }
};