#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GamePlay/CoreType/GameGrid.h"
#include "GridComponentInterface.generated.h"

/**
 * 格子组件接口
 * 1.可以获取最新的格子状态
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UGridComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 格子组件接口
 * 1.可以获取最新的格子状态
 */
class GAMEREVERSETIME_API IGridComponentInterface
{
	GENERATED_BODY()

public:

	//通过格子点位获取对应格子的类型
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GridComponentInterface")
	EGridType GetGridType(FGridPoint GridPoint);
	virtual EGridType GetGridType_Implementation(FGridPoint GridPoint) { return EGridType::Ground; }

	//通过格子点位设置对应格子的类型
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GridComponentInterface")
	void SetGridType(FGridPoint GridPoint, EGridType NewType);
	virtual void SetGridType_Implementation(FGridPoint GridPoint, EGridType NewType) {}

	//通过格子点位获取格子坐标位置
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GridComponentInterface")
	FVector2D GetGridLocation(FGridPoint GridPoint);
	virtual FVector2D GetGridLocation_Implementation(FGridPoint GridPoint) { return FVector2D(0.f); }

	//获取当前地图格子的行列（例如：7行，9列）
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GridComponentInterface")
	FGridPoint GetGridRowAndCol();
	virtual FGridPoint GetGridRowAndCol_Implementation() { return FGridPoint(); }

	//获取格子大小（例如：60px，60px）
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GridComponentInterface")
	FVector2D GetGridSize();
	virtual FVector2D GetGridSize_Implementation() { return FVector2D(0.f); }

	//获取格子有效范围
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GridComponentInterface")
	FGridInWorldRange GetGridRange();
	virtual FGridInWorldRange GetGridRange_Implementation() { return FGridInWorldRange(); }

};