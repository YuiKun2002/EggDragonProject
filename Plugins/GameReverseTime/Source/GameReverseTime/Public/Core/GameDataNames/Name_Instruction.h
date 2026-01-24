#pragma once
#include "CoreMinimal.h"
#include "Core/GameDataNameMacro.h"

//游戏数据名字资产
namespace GameDataNameAsset {

	//数据资产【Instruction】指令，游戏指令
	namespace GameDataAsset_GlobalAsset_Instruction {
		//数据资产——【Texture】纹理
		DEF_CATEGORYNAME(GlobalAsset_Instruction);
		//全部可用的指令
		DEF_NAME(All);
		//用于游戏场景的指令（GameVs）
		DEF_NAME(Game);
		//适用于角色存档的指令
		DEF_NAME(Player);
		//适用于礼盒模式的指令
		DEF_NAME(Gift);
		//适用于符合引用作为关键映射
		DEF_NAME(SymbolRef);
		//适用于符号的指令
		DEF_NAME(Symbol);
	}
}