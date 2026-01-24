#pragma once
#include "CoreMinimal.h"
#include "Core/GameDataNameMacro.h"

//游戏数据名字资产
namespace GameDataNameAsset {

	//数据资产【Data】一些特别的数据
	namespace GameDataAsset_GlobalAsset_Data {
		//数据资产——【Texture】纹理
		DEF_CATEGORYNAME(GlobalAsset_Data);
		//游戏道具的ID号集合
		DEF_NAME(GameID);
		//游戏道具的最大ID号和已经被删除的ID号
		DEF_NAME(ItemID);
		//装备道具整理类型数据表
		DEF_NAME(EquipArrange);
		//卡片道具整理类型数据表
		DEF_NAME(CardArrange);
		//材料道具整理类型数据表
		DEF_NAME(MaterialArrange);
	}
}