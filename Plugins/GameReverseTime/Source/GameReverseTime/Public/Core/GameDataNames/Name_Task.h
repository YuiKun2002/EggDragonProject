#pragma once
#include "CoreMinimal.h"
#include "Core/GameDataNameMacro.h"

//游戏数据名字资产
namespace GameDataNameAsset {


	//数据资产【任务】
	namespace GameDataAsset_GlobalAsset_Task {
		//数据资产——任务分类
		DEF_CATEGORYNAME(GlobalAsset_Task);
		//主线任务
		DEF_NAME(MainTask);
		//每日任务
		DEF_NAME(DayTask);
		//挑战任务
		DEF_NAME(ChallengeTask);
		//角色升级任务
		DEF_NAME(PlayerUpTask);



		//任务实现的数据表

		//关卡任务
		DEF_NAME(Imp_Task_Level);
		//卡片制作
		DEF_NAME(Imp_Task_MakeCard);
		//卡片强化
		DEF_NAME(Imp_Task_CardUpgrade);
		//技能书
		DEF_NAME(Imp_Task_CardSkillBooks);
		//宝石
		DEF_NAME(Imp_Task_Gem);
		//情报岛
		DEF_NAME(Imp_Task_Information);
		//角色升级
		DEF_NAME(Imp_Task_PlayerUpgrade);
	}
}