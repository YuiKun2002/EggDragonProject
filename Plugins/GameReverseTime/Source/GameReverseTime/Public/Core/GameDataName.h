#pragma once
#include "GameObject.h"
#include "GameDataNameMacro.h"//名称宏
/*
	各种资产名称
*/
#include "GameDataNames/Name_UserInterface.h"//UI
#include "GameDataNames/Name_PlayerStructManager.h"//角色存档
#include "GameDataNames/Name_Dialogue.h"//剧情
#include "GameDataNames/Name_Card.h"//卡片
#include "GameDataNames/Name_Material.h"//材料
#include "GameDataNames/Name_Equip.h"//装备道具
#include "GameDataNames/Name_Mail.h"//邮件
#include "GameDataNames/Name_GameVS.h"//游戏关卡
#include "GameDataNames/Name_Task.h"//任务
#include "GameDataNames/Name_Texture.h"//纹理贴图
#include "GameDataNames/Name_Instruction.h"//指令
#include "GameDataNames/Name_OtherData.h"//其他数据
#include "GameDataName.generated.h"

/*
	可视化名字类型分类[蓝图使用]
	1.可以更好的就行名称统一
	2.方便查找和调试
	3.减少名称出错的问题
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEREVERSETIME_API UAssetCategoryName : public UGameObject {
	GENERATED_BODY()
public:
	//获取分类名称
	UFUNCTION(BlueprintImplementableEvent)
	FName GetCategoryName();
};

//C++使用
//在"GameDataNameMacro.h"//名称宏文件中


//全局名称-命名空间
namespace GameDataNameAsset {

}


/**
 * 全局资源名称
 *
 *  如果需要修改，请修改【蓝图实现BP_xxxx_CateName】【C++的UI宏名称】
 */
 /*
	 UI的分类名称

	 如果需要修改，请修改【数据表DT_GameUserInterface】【蓝图实现BP_xxxx_CateName】【C++的UI宏名称】

	 资产分为：数据资产GET_DEF_NAME()和UI资产GET_UI_NAME()

 */
UCLASS()
class GAMEREVERSETIME_API UGameDataName : public UObject {
	GENERATED_BODY()
public:

	//获取逆转时光项目使用的包ID
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Package ID"))
	static FName GetAsset_Package_ID() { return FName(*FString("GameReverseTime")); }

	//Tag资产全局名称
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name Global Asset UI"))
	static FName GetAsset_UI() { return GET_DEF_CATEGORYNAME(UI); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name Global Asset Card"))
	static FName GetAsset_Card() { return GET_DEF_CATEGORYNAME(Card); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name Global Asset Equip"))
	static FName GetAsset_Equip() { return GET_DEF_CATEGORYNAME(Equip); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name Global Asset GameVS"))
	static FName GetAsset_GameVS() { return GET_DEF_CATEGORYNAME(GameVS); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name Global Asset Material"))
	static FName GetAsset_Material() { return GET_DEF_CATEGORYNAME(Material); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name Global Asset Player"))
	static FName GetAsset_Player() { return GET_DEF_CATEGORYNAME(Player); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name Global Asset Task"))
	static FName GetAsset_Task() { return GET_DEF_CATEGORYNAME(Task); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name Global Asset Dialogue"))
	static FName GetAsset_Dialogue() { return GET_DEF_CATEGORYNAME(Dialogue); }
public:
	//Tag全局UI
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name UI Global"))
	static FName Get_UI_Global() { return GET_UI_CATEGORYNAME(Global); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global GameInfor"))
	static FName Get_UI_GameInfor() { return GET_UI_NAME(Global, GameInfor); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global DialogueInfor"))
	static FName Get_UI_DialogueInfor() { return GET_UI_NAME(Global, DialogueInfor); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global GamePrepare"))
	static FName Get_UI_GamePrepare() { return GET_UI_NAME(Global, GamePrepare); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global GameSetting"))
	static FName Get_UI_GameSetting() { return GET_UI_NAME(Global, GameSetting); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global GameTask"))
	static FName Get_UI_GameTask() { return GET_UI_NAME(Global, GameTask); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global GameTest"))
	static FName Get_UI_GameTest() { return GET_UI_NAME(Global, GameTest); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global GameWorldMap"))
	static FName Get_UI_GameWorldMap() { return GET_UI_NAME(Global, GameWorldMap); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global InGame"))
	static FName Get_UI_InGame() { return GET_UI_NAME(Global, InGame); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global ItemTitleTip"))
	static FName Get_UI_ItemTitleTip() { return GET_UI_NAME(Global, ItemTitleTip); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global MainFrame"))
	static FName Get_UI_MainFrame() { return GET_UI_NAME(Global, MainFrame); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global NewItemShow"))
	static FName Get_UI_NewItemShow() { return GET_UI_NAME(Global, NewItemShow); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global NewLevelTip"))
	static FName Get_UI_NewLevelTip() { return GET_UI_NAME(Global, NewLevelTip); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global Notice"))
	static FName Get_UI_Notice() { return GET_UI_NAME(Global, Notice); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global OpenGame"))
	static FName Get_UI_OpenGame() { return GET_UI_NAME(Global, OpenGame); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName_UI Global Synthesis"))
	static FName Get_UI_Synthesis() { return GET_UI_NAME(Global, Synthesis); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global TaskFinishTip"))
	static FName Get_UI_TaskFinishTip() { return GET_UI_NAME(Global, TaskFinishTip); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global Dialogue"))
	static FName Get_UI_Dialogue() { return GET_UI_NAME(Global, Dialogue); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global Mail"))
	static FName Get_UI_MailView() { return GET_UI_NAME(Global, Mail); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global PlayerBag"))
	static FName Get_UI_PlayerBagView() { return GET_UI_NAME(Global, PlayerBag); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global Pay"))
	static FName Get_UI_PayUI() { return GET_UI_NAME(Global, Pay); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Global SelectTip"))
	static FName Get_UI_SelectTipUI() { return GET_UI_NAME(Global, SelectTip); }
public:
	//Tag角色背包UI
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name UI PlayerBag"))
	static FName Get_UI_PlayerBag() { return GET_UI_CATEGORYNAME(PlayerBag); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI PlayerBag CardGrid"))
	static FName Get_UI_CardGrid() { return GET_UI_NAME(PlayerBag, CardGrid); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI PlayerBag EquipmentGrid"))
	static FName Get_UI_EquipmentGrid() { return GET_UI_NAME(PlayerBag, EquipmentGrid); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI PlayerBag ItemTip"))
	static FName Get_UI_ItemTip() { return GET_UI_NAME(PlayerBag, ItemTip); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI PlayerBag MaterialGrid"))
	static FName Get_UI_MaterialGrid() { return GET_UI_NAME(PlayerBag, MaterialGrid); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI PlayerBag PlayerShow"))
	static FName Get_UI_PlayerShow() { return GET_UI_NAME(PlayerBag, PlayerShow); }
public:
	//Tag设置UI
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name UI Setting"))
	static FName Get_UI_Setting() { return GET_UI_CATEGORYNAME(Setting); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Setting SettingButtonTab"))
	static FName Get_UI_SettingButtonTab() { return GET_UI_NAME(Setting, SettingButtonTab); }
public:
	//Tag世界地图UI
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name UI WorldMap"))
	static FName Get_UI_WorldMap() { return GET_UI_CATEGORYNAME(WorldMap); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI WorldMap MainMap"))
	static FName Get_UI_MainMap() { return GET_UI_NAME(WorldMap, MainMap); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI WorldMap Map1"))
	static FName Get_UI_Map1() { return GET_UI_NAME(WorldMap, Map1); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI WorldMap Map2"))
	static FName Get_UI_Map2() { return GET_UI_NAME(WorldMap, Map2); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI WorldMap Map3"))
	static FName Get_UI_Map3() { return GET_UI_NAME(WorldMap, Map3); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI WorldMap Map4"))
	static FName Get_UI_Map4() { return GET_UI_NAME(WorldMap, Map4); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI WorldMap Map5"))
	static FName Get_UI_Map5() { return GET_UI_NAME(WorldMap, Map5); }
public:
	//Tag关卡UI
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name UI GameVs"))
	static FName Get_UI_GameVs() { return GET_UI_CATEGORYNAME(GameVs); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI GameVs BottomTextTip"))
	static FName Get_UI_BottomTextTip() { return GET_UI_NAME(GameVs, BottomTextTip); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI GameVs GameOver"))
	static FName Get_UI_GameOver() { return GET_UI_NAME(GameVs, GameOver); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI GameVs GamePlayerHead"))
	static FName Get_UI_GamePlayerHead() { return GET_UI_NAME(GameVs, GamePlayerHead); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI GameVs GameTimer"))
	static FName Get_UI_GameTimer() { return GET_UI_NAME(GameVs, GameTimer); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI GameVs PlayPlayerTip"))
	static FName Get_UI_PlayPlayerTip() { return GET_UI_NAME(GameVs, PlayPlayerTip); }
public:
	//Tag任务
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName Task MainTask"))
	static FName Get_Task_MainTask() { return GET_DEF_NAME(Task, MainTask); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName Task DayTask"))
	static FName Get_Task_DayTask() { return GET_DEF_NAME(Task, DayTask); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName Task ChallengeTask"))
	static FName Get_Task_ChallengeTask() { return GET_DEF_NAME(Task, ChallengeTask); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName Task PlayerUpTask"))
	static FName Get_Task_PlayerUpTask() { return GET_DEF_NAME(Task, PlayerUpTask); }
public:
	//Tag UI 对话
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name UI Dialogue"))
	static FName Get_UI_DialogueView() { return GET_UI_CATEGORYNAME(Dialogue); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Dialogue DialogueSelect"))
	static FName Get_UI_DialogueSelect() { return GET_UI_NAME(Dialogue, DialogueSelect); }

public:
	//Tag UI 邮件
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name UI Mail"))
	static FName Get_UI_Mail() { return GET_UI_CATEGORYNAME(Mail); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Mail MessageBox"))
	static FName Get_UI_MailBox() { return GET_UI_NAME(Mail, MessageBox); }

public:
	//Tag UI 提示
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Category Name UI Tip"))
	static FName Get_UI_Tip() { return GET_UI_CATEGORYNAME(Tip); }
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get DataName UI Tip DesTip"))
	static FName Get_UI_Tip_DesTip() { return GET_UI_NAME(Tip, DesTip); }
};