#pragma once
#include "CoreMinimal.h"
#include "Core/GameDataNameMacro.h"

//游戏数据名字资产
namespace GameDataNameAsset {

	//数据资产【GameVS】关卡进行
	namespace GameDataAsset_GlobalAsset_GameVS {
		//数据资产——【GameVS】关卡进行分类
		DEF_CATEGORYNAME(GlobalAsset_GameVS);
		//Buff
		DEF_NAME(Buff);
		//关卡地图表格
		DEF_NAME(GameLevel);
		//地形放置卡片
		DEF_NAME(LevelPlayPreCard);
		//游戏角色
		DEF_NAME(GamePlayer);

	}
}