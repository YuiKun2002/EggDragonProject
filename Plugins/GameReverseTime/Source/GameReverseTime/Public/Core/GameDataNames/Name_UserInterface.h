#pragma once
#include "CoreMinimal.h"
#include "Core/GameDataNameMacro.h"

//游戏数据名字资产
namespace GameDataNameAsset {

	//数据资产【UI】
	namespace GameDataAsset_GlobalAsset_UI {
		//数据资产——UI分类
		DEF_CATEGORYNAME(GlobalAsset_UI);
		//数据资产的子项目由UI接口子系统自行遍历，自动添加项目
		//无需自此出新增项目
	}
}

namespace GameDataNameAsset {
	//UI资产 全局UI
	namespace UI_Global {
		//全局UI分类
		DEF_CATEGORYNAME(Global);
		//情报岛UI
		DEF_NAME(GameInfor);
		//任务UI
		DEF_NAME(GameTask);
		//任务完成提示
		DEF_NAME(TaskFinishTip);
		//解锁新关卡提示
		DEF_NAME(NewLevelTip);
		//新物品显示
		DEF_NAME(NewItemShow);
		//剧情对话信息
		DEF_NAME(DialogueInfor);
		//城镇UI操作主面板
		DEF_NAME(MainFrame);
		//世界地图UI
		DEF_NAME(GameWorldMap);
		//公告
		DEF_NAME(Notice);
		//设置UI
		DEF_NAME(GameSetting);
		//打开游戏【视频UI】
		DEF_NAME(OpenGame);
		//进入游戏主界面
		DEF_NAME(InGame);
		//测试版本的提示文字
		DEF_NAME(GameTest);
		//VS准备界面
		DEF_NAME(GamePrepare);
		// Windows平台鼠标移入时显示的物品文字
		DEF_NAME(ItemTitleTip);
		//合成屋
		DEF_NAME(Synthesis);
		//剧情
		DEF_NAME(Dialogue);
		//邮件
		DEF_NAME(Mail);
		//玩家背包
		DEF_NAME(PlayerBag);
		//支付界面
		DEF_NAME(Pay);
		//选择提示UI
		DEF_NAME(SelectTip);
		//未解锁的物品提示UI
		DEF_NAME(NotUnlockedItemTip);
	};

	//UI资产 角色背包UI
	namespace UI_PlayerBag {
		//角色背包分类
		DEF_CATEGORYNAME(PlayerBag);
		//卡片格子
		DEF_NAME(CardGrid);
		//装备格子
		DEF_NAME(EquipmentGrid);
		//材料格子
		DEF_NAME(MaterialGrid);
		//角色形象显示
		DEF_NAME(PlayerShow);
		//物品的展示信息
		DEF_NAME(ItemTip);
		//卡片物品的展示信息
		DEF_NAME(ItemCardTip);
		//物品的展示信息【礼包(开箱子)专属界面】
		DEF_NAME(GiftItemTip);
		//物品的展示信息【礼包(开箱子)概率项目展示界面】
		DEF_NAME(GiftItemTip_Item);
		//装备格子-定制格子-背包道具专属
		DEF_NAME(InventoryGrid);
		//角色信息面板
		DEF_NAME(PlayerInforPanel);
		//物品排序设置UI
		DEF_NAME(Arrangement);
		//物品排序项目
		DEF_NAME(ArrangementItem);
		//物品排序项目设置细节面板
		DEF_NAME(ArrangementSetting);
		//技能书选项卡标签
		DEF_NAME(SkillBookTab);
	}

	//UI资产 设置UI
	namespace UI_Setting
	{
		//设置UI分类
		DEF_CATEGORYNAME(Setting);
		//设置UI选项卡
		DEF_NAME(SettingButtonTab);
	}

	//UI资产 地图UI
	namespace UI_WorldMap {
		//地图UI分类
		DEF_CATEGORYNAME(WorldMap);
		//城镇地图
		DEF_NAME(MainMap);
		//美味岛
		DEF_NAME(Map1);
		//火山岛
		DEF_NAME(Map2);
		//火山遗迹
		DEF_NAME(Map3);
		//浮空岛
		DEF_NAME(Map4);
		//海底
		DEF_NAME(Map5);
	}

	//UI资产 游戏战斗UI
	namespace UI_GameVs
	{
		//设置UI分类
		DEF_CATEGORYNAME(GameVs);
		//放置角色的提升UI
		DEF_NAME(PlayPlayerTip);
		//底部文字提示
		DEF_NAME(BottomTextTip);
		//关卡倒计时
		DEF_NAME(GameTimer);
		//游戏结束结算界面
		DEF_NAME(GameOver);
		//角色头像
		DEF_NAME(GamePlayerHead);
		//卡片栏
		DEF_NAME(CardBar);
		//卡片UI
		DEF_NAME(CardUI);
	}


	//UI资产 剧情对话UI
	namespace UI_Dialogue
	{
		//设置UI分类
		DEF_CATEGORYNAME(Dialogue);
		//对话选项
		DEF_NAME(DialogueSelect);
	}

	//UI资产 邮件UI
	namespace UI_Mail
	{
		//邮件UI分类
		DEF_CATEGORYNAME(Mail);
		//邮件列表项目
		DEF_NAME(MessageBox);
		//邮件领取邮件的界面
		DEF_NAME(GiftBox);
	}

	//UI资产 提示UI
	namespace UI_Tip {
		//提示UI分类
		DEF_CATEGORYNAME(Tip);
		//玩家获得道具的提示框
		DEF_NAME(PlayerItemTipBox);
		//玩家获得道具的提示框-提示框项目
		DEF_NAME(PlayerItemTipBoxItem);
		//描述提示界面
		DEF_NAME(DesTip);
	}

	//UI资产 合成屋UI
	namespace UI_Synthesis {
		//合成屋UI
		DEF_CATEGORYNAME(Synthesis);
		//合成屋道具格子-道具
		DEF_NAME(SynItemGrid);
		//合成屋道具格子-卡片
		DEF_NAME(SynCardGrid);
		//卡片制作主界面
		DEF_NAME(MakeCard);
		//卡片强化主界面
		DEF_NAME(UpgradeCard);
		//卡片转职主界面
		DEF_NAME(TransferCard);
		//武器宝石镶嵌界面
		DEF_NAME(WeaponAndGem);
		//宝石强化主界面
		DEF_NAME(UpgradeGem);
		//宝石分解主界面
		DEF_NAME(SplitGem);
		//金卡进化主界面
		DEF_NAME(GoldCardEvolve);
	}
}