#pragma once

#include "Interface/DataTable/ItemDataInterface.h"
#include "Core/DataTable/Item/Card.h"
#include "CardDataInterface.generated.h"

/**
 * 卡片数据接口
 */
UINTERFACE(BlueprintType)
class GAMEREVERSETIME_API UCardDataInterface : public UItemDataInterface
{
	GENERATED_BODY()
};

/**
 * 卡片数据接口
 */
class GAMEREVERSETIME_API ICardDataInterface : public IItemDataInterface
{
	GENERATED_BODY()

public:

	//获取卡片数据
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CardData")
	bool GetCard(int32 ItemID, FCardTableRow& OutData);
	virtual bool GetCard_Implementation(int32 ItemID, FCardTableRow& OutData);

	//获取所有的卡片数据
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CardData")
	void GetCards(TMap<int32, FCardTableRow>& OutCards);
	virtual void GetCards_Implementation(TMap<int32, FCardTableRow>& OutCards) {}

};
