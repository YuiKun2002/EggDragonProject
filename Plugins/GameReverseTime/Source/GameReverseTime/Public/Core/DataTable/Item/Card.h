#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/DataTable/Item/Item.h"
#include "Card.generated.h"


//卡片转职进化
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FCardEvolution {

	GENERATED_USTRUCT_BODY()

	//转职的目标ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EvolveCardName;

	//转职所需材料ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FString> EvolveMaterials;

	//转职所需等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 EvolveLevel = 9;
};

//卡片基础底层
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FCardData {

	GENERATED_USTRUCT_BODY()

	//卡片的大头像
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath CardBarAvatar;
	//卡片拖拽动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperFlipbook> CardDrogAnim;
	//数据关联
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<TSoftObjectPtr<class UObjectAbilityDetailDataAsset>> CardDetailDatas;
	//卡片对应的蓝图实例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class AGameRoleActor> CardRoleActorAsset;
	//卡片放置类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 PlaceType = 0U;
	//卡片生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP = 10.f;
	//卡片防御
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DEF = 0;
	//冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CD = 7.f;
	//是夜晚类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsNight = false;
	//卡片价格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price = 150;
	//设置自增价格，设置之后每放置一张防御卡价格自动提升！
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PriceIncrement = 0;
	//高级卡片[典型例子：会直接影响强化概率的高低，设置true直接视为高概率卡片]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPremiumCard = false;
	//卡片优先级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlaceLayerLevel = 0;
	//替换层级[如果设置了替换层级，对应的卡片会触发替换规则]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<int32> ReplaceLayers;
	//卡片转职
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCardEvolution Evolve;
};

//卡片数据表格
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FCardTableRow : public FItemTableRow
{
	GENERATED_USTRUCT_BODY();

	FCardTableRow() { this->ItemData.Type = 1U; }

	//物品数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData ItemData;

	//卡片数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCardData CardData;

	virtual FItemData GetItemData() { return this->ItemData; }
};
