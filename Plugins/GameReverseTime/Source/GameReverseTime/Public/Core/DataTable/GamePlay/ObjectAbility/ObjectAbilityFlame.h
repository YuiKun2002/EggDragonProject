#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObjectAbilityFlame.generated.h"

//能力火苗数据表
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FObjectAbilityFlameTableRow : public FTableRowBase {

	GENERATED_USTRUCT_BODY()

	//火苗产量值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectAbilityFlameTableRow")
	int32 FlameValue = 25;

	//火苗生产数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectAbilityFlameTableRow")
	int32 SpawnCount = 1;
};
