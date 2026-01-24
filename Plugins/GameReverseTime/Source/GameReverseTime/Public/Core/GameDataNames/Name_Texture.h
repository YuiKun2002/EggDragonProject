#pragma once
#include "CoreMinimal.h"
#include "Core/GameDataNameMacro.h"

//游戏数据名字资产
namespace GameDataNameAsset {

	//数据资产【Texture】纹理，一般是常用纹理
	namespace GameDataAsset_GlobalAsset_Texture {
		//数据资产——【Texture】纹理
		DEF_CATEGORYNAME(GlobalAsset_Texture);
		//物品的等级 1-16级，小图标
		DEF_NAME(ItemLevelLog);
		//货币【金币，礼券，点券，威望等等】
		DEF_NAME(Coin);
		//玩家等级纹理
		DEF_NAME(PlayerLevelTexture);
	}
}