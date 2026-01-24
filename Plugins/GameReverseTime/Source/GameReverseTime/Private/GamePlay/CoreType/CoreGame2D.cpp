#include "GamePlay/CoreType/CoreGame2D.h"

FVector UCoreGame2D::Vector2DConvVector(const FVector2D& Vc2D)
{
    return FV2DTOVE(Vc2D);
}

FVector2D UCoreGame2D::VectorConvVector2D(const FVector& Vc)
{
    return FVETOV2D(Vc);
}

FTransform UCoreGame2D::MakeTransform(const FVector2D& Vc2D, float Angle)
{
    return TRANS2D(Vc2D,Angle);
}

float UCoreGame2D::GetRotationAngle(const FRotator& Rota)
{
    return FROTOANGLE(Rota);
}

FRotator UCoreGame2D::MakeRotator(float Angle)
{
    return FRO(Angle);
}
