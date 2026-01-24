#include "GamePlay/GameRole/GameRoleActor.h"
#include "GameFramework/GameStateBase.h"
#include "GamePlay/GameManage/Interface/GamePlayStateInterface.h"

AGameRoleActor::AGameRoleActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

FVector2D AGameRoleActor::GetGridPointLocation()const
{
	//获取格子的位置
	FGridPoint Grid = this->GetGridPoint();

	//计算列=起始位置+格子大小的列宽*列+列宽/2

	// X Col ，Y Row
	return FVector2D(
		GlobalGridInWorldRange.HorizontalLeftPoint + GlobalGridSize.X * Grid.Col + GlobalGridSize.X / 2.f,
		GlobalGridInWorldRange.VerticalTopPoint + GlobalGridSize.Y * Grid.Row + GlobalGridSize.Y / 2.f
	);
}

AActor* AGameRoleActor::GetGameManageActor() {

	class AGameStateBase* GameState = this->GetWorld()->GetGameState();
	IGamePlayStateInterface* Inter = dynamic_cast<IGamePlayStateInterface*>(GameState);
	if (Inter)
	{
		return Inter->Execute_GetGameManage(GameState);
	}
	return nullptr;
}