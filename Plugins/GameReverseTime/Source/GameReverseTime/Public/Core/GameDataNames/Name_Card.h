#pragma once
#include "CoreMinimal.h"
#include "Core/GameDataNameMacro.h"

//游戏数据名字资产
namespace GameDataNameAsset {

	//数据资产【Card】卡片
	namespace GameDataAsset_GlobalAsset_Card {
		//数据资产——Card卡片分类
		DEF_CATEGORYNAME(GlobalAsset_Card);
		//卡片数据表集合
		DEF_NAME(CardDataTables);
		//攻击卡
		DEF_NAME(ATK);
		//生产卡
		DEF_NAME(Spawn);
		//防御卡
		DEF_NAME(Defence);
		//功能卡
		DEF_NAME(Function);
		//范围攻击卡
		DEF_NAME(RangeATK);
	}
}