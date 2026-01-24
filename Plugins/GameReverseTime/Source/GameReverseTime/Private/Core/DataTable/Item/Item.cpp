#include "Core/DataTable/Item/Item.h"
#include "Core/GameDataName.h"
FName ItemCategoryTypeConvToName(const EItemCategoryType& CategoryType)
{
	switch (CategoryType)
	{
	case EItemCategoryType::Card: return GET_DEF_CATEGORYNAME(Card);
	case EItemCategoryType::Item: return GET_DEF_CATEGORYNAME(Equip);
	case EItemCategoryType::Mater: return GET_DEF_CATEGORYNAME(Material);
	default: return FName();
	}
	return FName();
}
