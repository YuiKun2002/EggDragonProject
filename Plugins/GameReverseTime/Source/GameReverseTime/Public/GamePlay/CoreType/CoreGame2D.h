#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CoreGame2D.generated.h"

//向量-将一个FVector2D转换成适用于当前项目的坐标
#define FV2DTOVE(V2d) FVector(V2d.X,0.f,V2d.Y)
//向量-将一个FVector转换成适用于当前项目的FVector2D坐标
#define FVETOV2D(Vec) FVector2D(Vec.X,Vec.Z)

//旋转
#define FRO(Angle) FRotator(Angle,0.f,0.f)
#define FROTOANGLE(Rotator) Rotator.Pitch

/*
	变换坐标
	1.Location
	2.Rotation
*/
#define TRANS2D(V2d,RotationValue) FTransform(FQuat::MakeFromEuler(FVector(0.0f, RotationValue, 0.f)),FV2DTOVE(V2d))



/*
	核心静态函数
*/
UCLASS()
class GAMEREVERSETIME_API UCoreGame2D : public UObject
{
	GENERATED_BODY()

public:

	//将3d向量转换为适用当前项目的2d向量
	UFUNCTION(BlueprintPure, Category = "CoreGame2D")
	static FVector Vector2DConvVector(const FVector2D& Vc2D);

	//将2d向量转换为适用当前项目的3d向量
	UFUNCTION(BlueprintPure, Category = "CoreGame2D")
	static FVector2D VectorConvVector2D(const FVector& Vc);

	//创建变换
	UFUNCTION(BlueprintPure, Category = "CoreGame2D")
	static FTransform MakeTransform(const FVector2D& Vc2D,float Angle);

	//获取旋转角度
	UFUNCTION(BlueprintPure, Category = "CoreGame2D")
	static float GetRotationAngle(const FRotator& Rota);

	//创建角度
	UFUNCTION(BlueprintPure, Category = "CoreGame2D")
	static FRotator MakeRotator(float Angle);
};
