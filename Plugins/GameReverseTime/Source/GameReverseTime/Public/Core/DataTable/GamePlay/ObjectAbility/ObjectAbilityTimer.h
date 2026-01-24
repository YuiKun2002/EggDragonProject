#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/NoExportTypes.h"
#include "ObjectAbilityTimer.generated.h"

//计时器数据表
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FObjectAbilityTimerTableRow : public FTableRowBase {

	GENERATED_USTRUCT_BODY()

	//初始计时
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectAbilityTimerTableRow")
	float FristTimeValue = 0.f;

	//最大计时
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectAbilityTimerTableRow")
	float MaxTimer = 10.f;

	//计时循环
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectAbilityTimerTableRow")
	bool bLoop = true;
};
