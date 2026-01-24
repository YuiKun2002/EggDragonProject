#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

//物品分类类型
UENUM(BlueprintType)
enum class EItemCategoryType : uint8 {
	//物品类型-装备类型
	Item UMETA(DisplayName = "Item"),
	//卡片类型
	Card UMETA(DisplayName = "Card"),
	//材料类型
	Mater UMETA(DisplayName = "Mater"),


	//类型最大值[如果有循环需求]
	Max = Mater + 1U UMETA(DisplayName = "TypeMax")
};

//物品分类枚举类型-转换为-名称
GAMEREVERSETIME_API FName ItemCategoryTypeConvToName(const EItemCategoryType& CategoryType);

//物品结构
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FItemData {

	GENERATED_USTRUCT_BODY()

	//道具名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Name;

	//道具类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	uint8 Type = 0U;

	//物品等级[在编辑器中不需要手动设置]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Level = 0;

	//道具描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Describle;

	//显示的纹理路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FSoftObjectPath TexturePath;

	//全局物品名称ID[动态初始化]
	UPROPERTY(VisibleAnywhere)
	FString GlobalPackageItemName;
};

/*物品表结构
1.用于表示物品在数据表中的结构
*/
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FItemTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	//物品ID号
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemTableRow")
	int32 ItemID = 0;

	void SetID(int32 ID) {
		this->ItemID = ID;
	}

	int32 GetID() const {
		return this->ItemID;
	}

	virtual FItemData GetItemData() { return FItemData(); }
};