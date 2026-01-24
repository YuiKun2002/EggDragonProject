#include "Core/GameDataStaticObjectSpawnImp.h"
#include "Core/GameObject.h"

namespace GameDataStaticObjects {
	//定义命名空间中的变量
	TMap<FName, UGameDataStaticObject*> StaticPools = {};
	TSharedPtr<GameDataStaticObjectSpawn> Spawn = MakeShareable(new GameDataStaticObjectSpawn());
	GameDataStaticObjectSpawn::~GameDataStaticObjectSpawn() {
		this->RecycleGarbage("GameEnd");
	}
	void GameDataStaticObjectSpawn::RecycleGarbage(FString Tip) {
		//UE_LOG(LogTemp, Log, TEXT("%s回收游戏静态对象内存:"), *Tip);
		//回收掉静态游戏数据
		for (const auto& StaticObj : StaticPools)
		{
			if (IsValid(StaticObj.Value))
			{
				//UE_LOG(LogTemp, Log, TEXT("\t\t%s"), *(StaticObj.Value->GetStaticName().ToString()));
				StaticObj.Value->RemoveFromRoot();
				StaticObj.Value->MarkAsGarbage();
			}
		}
		StaticPools.Empty(0);
	}
}

