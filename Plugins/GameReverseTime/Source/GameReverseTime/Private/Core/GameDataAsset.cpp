#include "Core/GameDataAsset.h"
#include "GameReverseTime.h"
#include "Core/GameDataSubsystem.h"

UDataTable* UGameDataAssetObject::GetDataByName(FName Name)
{
	return this->GetDataByName_Imp(MoveTemp(Name));
}

UDataTable* UGameDataAssetObject::GetDataByName_Imp(GameDataName&& Name)
{
	UDataTable** TargetTable = this->DataInstance.Find(Name);
	if (TargetTable && IsValid(*TargetTable))
	{
		return *TargetTable;
	}

	TSoftObjectPtr<UDataTable>* TargetSource = this->Datas.Find(Name);
	if (TargetSource)
	{
		UDataTable* NewData = TargetSource->LoadSynchronous();
		if (IsValid(NewData))
		{
			this->DataInstance.Emplace(Name, NewData);

			return NewData;
		}
	}

#if WITH_EDITOR
	UE_LOG(GameReverseTime,
		Error,
		TEXT("[UGameDataAssetObject::GetDataByName_Imp]数据表加载失败，数据表{%s}无效"),
		*Name.ToString());
#endif

	return nullptr;
}

void UGameDataAssetObject::Unload()
{
	for (auto& Instance : this->DataInstance)
	{
		if (IsValid(Instance.Value))
		{
			Instance.Value = nullptr;
		}
	}

	this->DataInstance.Empty();
}

UGameDataAssetCache* UGameDataAssetCache::MakeGameDataAssetCache(const UObject* WorldContextObject, TSoftClassPtr<UGameDataAssetCache> BpGameDataAssetCache)
{
	//使用静态加载
	UGameDataAssetCache* CurrentCache = TSubclassOf_StaticLoadObject<UGameDataAssetCache>(
		BpGameDataAssetCache, WorldContextObject
	);

	if (IsValid(CurrentCache))
	{
		UGameDataSubsystem* TempGameDataSubsystem = UGameDataSubsystem::GetGameDataSubsystemStatic(WorldContextObject);
		UGameDataAssetCache* CurCache = TempGameDataSubsystem->GetGameDataAssetCache_Imp(
			CurrentCache->BP_GetNameTag()
		);
		if (IsValid(CurCache))
		{
#if WITH_EDITOR
			//判断重名问题
			if (CurCache->bIsBlueprint())
			{

				ensureMsgf(false, *FString::Printf(TEXT("数据资产重名[%s]，请重新设置名称"),
					*CurrentCache->BP_GetNameTag().ToString()));
				UE_LOG(GameReverseTime, Log, TEXT("数据资产重名[%s]，请重新设置名称"),
					*CurrentCache->BP_GetNameTag().ToString());

				return nullptr;
			}
#endif
			CurCache->SetWorldContext(WorldContextObject);
			return CurCache;
		}
#if WITH_EDITOR
		CurrentCache->bBp = true;
#endif
		CurrentCache->SetWorldContext(WorldContextObject);
		CurrentCache->InitCache(TempGameDataSubsystem);
		TempGameDataSubsystem->AddGameDataAssetCache_Imp(CurrentCache->BP_GetNameTag(), CurrentCache);
		return CurrentCache;
	}

	return nullptr;
}

void UGameDataAssetCache::InitCache_Implementation(UGameDataSubsystem* GameDataSubsystem) {
	//UE_LOG(LogTemp, Warning, TEXT("[UGameDataAssetCache,%p]：调用了C++的默认初始化函数"), this);
}

void UGameDataAssetCache::Unload_Implementation() {
	//UE_LOG(LogTemp, Warning, TEXT("[UGameDataAssetCache,%p]：调用了C++的默认资产缓存卸载函数"), this);
}


