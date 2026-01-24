#pragma once
#include "CoreMinimal.h"
#include "Core/GameDataNameMacro.h"

//游戏数据名字资产
namespace GameDataNameAsset {

	//数据资产【Material】材料
	namespace GameDataAsset_GlobalAsset_Material {
		//数据资产——Material材料分类
		DEF_CATEGORYNAME(GlobalAsset_Material);
		//卡片配方
		DEF_NAME(Blueprint);
		//卡片配方材料
		DEF_NAME(BlueprintMater);
		//转职
		DEF_NAME(Change);
		//香料
		DEF_NAME(Spices);
		//四叶草
		DEF_NAME(Clover);
		//防御卡技能书
		DEF_NAME(SkillBook);
		//兑换券
		DEF_NAME(Ticket);
		//强化水晶
		DEF_NAME(Crystal);
		//钻头
		DEF_NAME(Bit);
		//关卡钥匙和徽章
		DEF_NAME(LevelKey);
		//金卡进化材料匹配表格
		DEF_NAME(GoldCardUpData);
		//防御卡技能书匹配表格
		DEF_NAME(SkillBookData);
	}
}