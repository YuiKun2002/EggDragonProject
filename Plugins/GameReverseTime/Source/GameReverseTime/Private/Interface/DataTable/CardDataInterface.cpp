#include "Interface/DataTable/CardDataInterface.h"

bool ICardDataInterface::GetCard_Implementation(int32 ItemID, FCardTableRow& OutData)
{
	uint8 ItemType = 0U;
	void* Data = nullptr;
	if (this->GetItem(ItemID, ItemType, Data))
	{
		OutData = *(static_cast<FCardTableRow*>(Data));
		return true;
	}

	return false;
}
