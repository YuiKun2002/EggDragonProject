#include "TileViewArea.h"

TSharedRef<STableViewBase> UTileViewArea::RebuildListWidget()
{
	TSharedRef<STileViewArea<UObject*>> Ref = ConstructTileView<STileViewArea>();
	Ref->SetAllowMouseLeft(this->bEnableLeftClickScrolling);
	Ref->SetScrollbarVisibility(UWidget::ConvertSerializedVisibilityToRuntime(this->ScrollBarVisibility));
	Ref->SetStyle(&this->WidgetStyle);
	return Ref;
}

UUserWidget& UTileViewArea::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (this->bEnableLeftClickScrolling)
	{
		return GenerateTypedEntry<UUserWidget, SListViewAreaObjectTableRow<UObject*>>(DesiredEntryClass, OwnerTable);
	}

	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}
