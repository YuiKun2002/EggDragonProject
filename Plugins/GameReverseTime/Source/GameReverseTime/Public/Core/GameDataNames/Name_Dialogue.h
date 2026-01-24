#pragma once
#include "CoreMinimal.h"
#include "Core/GameDataNameMacro.h"

//游戏数据名字资产
namespace GameDataNameAsset {

	//数据资产【Dialogue】剧情
	namespace GameDataAsset_GlobalAsset_Dialogue {
		//数据资产——剧情分类
		DEF_CATEGORYNAME(GlobalAsset_Dialogue);
		//章节关联名称
		DEF_NAME(Chapter);
		//主线剧情对话列表
		DEF_NAME(MainLine);
		//支线剧情对话列表
		DEF_NAME(OtherLine);
	}
}