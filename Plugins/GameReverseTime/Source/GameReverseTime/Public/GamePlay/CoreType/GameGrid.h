#pragma once
#include "CoreMinimal.h"
#include "GameGrid.generated.h"

//全局的格子范围(初始化后，不需要通过组件获取)
extern GAMEREVERSETIME_API FGridInWorldRange GlobalGridInWorldRange;
//全局格子行列【不需要进行下标处理】
extern GAMEREVERSETIME_API FGridPoint GlobalGridRowCol;
//全局的格子大小(初始化后，不需要通过组件获取)
extern GAMEREVERSETIME_API FVector2D GlobalGridSize;

//格子类型
UENUM(BlueprintType)
enum class EGridType : uint8
{
	//陆地
	Ground,
	//水面
	Water,


	//....
	//无属性-无法放置
	Null
};

/*
	格子的区域
	1.记录玩家可操作区域
	2.用于各种物体的网格位置判断
	3.范围计算
	格子大小50px,50px，5行，5列，首个格子起始位置(0,0)
	最终范围
	VerticalTopPoint: 25.f        (0 + 50px / 2)
	VerticalBottomPoint: -250.f   (0 - 50px * 5行)
	HorizontalLeftPoint：25.f     (0 + 50px / 2)
	HorizontalRightPoint：250.f   (0 + 50px * 5列)

	可以直接简化为两个变量，起始坐标，偏移坐标。
	格子延申方向为起始位置向右，和起始位置向下
	FVector2D BeginLocation  = (HorizontalLeftPoint,VerticalTopPoint)
	FVector2D OffsetLocation = (HorizontalRightPoint,VerticalBottomPoint)
*/
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FGridInWorldRange
{
	GENERATED_BODY()

	//纵向顶部高度:Y
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalTopPoint = 0.f;

	//纵向底部高度:Y
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalBottomPoint = 0.f;

	//横向左边长度:X
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HorizontalLeftPoint = 0.f;

	//横向右边长度:X
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HorizontalRightPoint = 0.f;

};

//格子点位
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FGridPoint
{
	GENERATED_BODY()

	//格子行
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Row = -1;

	//格子列
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Col = -1;


	FGridPoint() :
		Row(-1),
		Col(-1) {
	}

	FGridPoint(const int32& NewPoint) :Row(NewPoint), Col(NewPoint) {}
	FGridPoint(const int32& NewRow, const int32& NewCol) :Row(NewRow), Col(NewCol) {}
	FGridPoint(const FGridPoint& NewPoint) :Row(NewPoint.Row), Col(NewPoint.Col) {}

	//有效判断
	FORCEINLINE bool IsValid() const { return (Row > -1) && (Col > -1); }

	//判断是否在范围内
	FORCEINLINE bool IsValidRange() const { return (Row > -1 && Row <= GlobalGridRowCol.Row) && (Col > -1 && Col <= GlobalGridRowCol.Col); }

	//赋值
	FGridPoint operator=(const FGridPoint& NewPoint) {
		this->Row = NewPoint.Row;
		this->Col = NewPoint.Col;
		return *this;
	}

	//判断
	bool operator==(const FGridPoint& NewPoint) {
		return (this->Row == NewPoint.Row) && (this->Col == NewPoint.Col);
	}

	//判断
	bool operator==(const FGridPoint& NewPoint) const {
		return (this->Row == NewPoint.Row) && (this->Col == NewPoint.Col);
	}

};


/*
	通过坐标获取网格点位
*/
#define GRIDPOINT(Vec) FGridPoint(\
		(int32)floorf((GlobalGridInWorldRange.VerticalTopPoint - Vec.Z) / GlobalGridSize.Y),\
		(int32)floorf((Vec.X - GlobalGridInWorldRange.HorizontalLeftPoint) / GlobalGridSize.X)\
)\




//格子数据表结构-用于编辑器填写的表
#if 1

//地图数据结构
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FGameMapDataList : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMapDataStructAsset> MapDataTable;

};


//关卡信息
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FGameMapInforConfig
{
	GENERATED_BODY()

	//地图名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	//初始火苗数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FlameInitCount = 100;

	//地图UI界面背景
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UTexture2D> MapWidgetBackground;

	//地图游戏关卡背景
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperSprite> MapGameBackground;


};

//关卡格子配置
USTRUCT(BlueprintType)
struct  GAMEREVERSETIME_API FGameMapGridConfig
{
	GENERATED_BODY()

	//首个网格位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D InitGridLocation = FVector2D(-140.f, 269.f);

	//格子大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D InitGridSize = FVector2D(60.f);

	//行
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Row = 6;

	//列
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Col = 7;

};

//关卡格子细节配置(如果不配置则默认)
USTRUCT(BlueprintType)
struct  GAMEREVERSETIME_API FGameMapGridDetailConfig
{
	GENERATED_BODY()

	//格子位置-行
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridRowPosition = 0;

	//格子位置-列
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GridColPosition = 0;

	//格子类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGridType GridType = EGridType::Ground;


};

//数据资产
UCLASS()
class GAMEREVERSETIME_API UMapDataStructAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	//关卡信息配置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameMapInforConfig InforConfig;

	//格子数据配置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameMapGridConfig GridConfig;

	//格子细节配置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameMapGridDetailConfig> GridDetailConfig;
};

#endif