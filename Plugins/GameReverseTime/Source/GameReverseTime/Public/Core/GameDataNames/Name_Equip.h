#pragma once
#include "CoreMinimal.h"
#include "Core/GameDataNameMacro.h"

//游戏数据名字资产
namespace GameDataNameAsset {
	//数据资产【Equip】装备
	namespace GameDataAsset_GlobalAsset_Equip {
		//数据资产——Equip装备分类
		DEF_CATEGORYNAME(GlobalAsset_Equip);
		//背包
		DEF_NAME(Bag);
		//礼盒
		DEF_NAME(GiftBox);
		//主武器
		DEF_NAME(WeaponFirst);
		//副武器
		DEF_NAME(WeaponSecond);
		//超级武器
		DEF_NAME(WeaponSuper);
		//武器宝石
		DEF_NAME(WeaponGem);
		//男-身体
		DEF_NAME(BodyMan);
		//女-身体
		DEF_NAME(BodyMen);
		//女-头发
		DEF_NAME(HairMen);
		//女-眼镜
		DEF_NAME(GlassesMen);
		//女-帽子
		DEF_NAME(CapMen);
		//不区分-帽子
		DEF_NAME(CapBase);
		//女-眼睛
		DEF_NAME(EyeMen);
		//不区分-眼睛
		DEF_NAME(EyeBase);
		//女-脸
		DEF_NAME(FaceMen);
		//男-翅膀
		DEF_NAME(FlyItemMan);
		//女-翅膀
		DEF_NAME(FlyItemMen);
		//不区分-翅膀
		DEF_NAME(FlyItemBase);
		//男-套装
		DEF_NAME(SuitMan);
		//女-套装
		DEF_NAME(SuitMen);
		//不区分-套装
		DEF_NAME(SuitBase);
	}
}