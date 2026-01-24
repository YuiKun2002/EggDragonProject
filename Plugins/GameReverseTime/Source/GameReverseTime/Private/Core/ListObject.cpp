#include "Core/ListObject.h"
#include "Components/ListView.h"

UListObject* UListObject::MakeListObject(class UListView* View, const TArray<UObject*>& Objects) {
	UListObject* ListObj = NewObject<UListObject>();
	ListObj->ResetBindData(Objects, View);
	return ListObj;
}

void UListObject::ForceRefresh() {
	this->ListViewPtr->RegenerateAllEntries();
}

void UListObject::ResetBindData(const TArray<UObject*>& Objects, class UListView* View) {
	if (IsValid(View))
	{
		this->ListViewPtr = nullptr;
		View->SetListItems(Objects);
		this->ListViewPtr = View;
	}
}

void UListObject::ResetBindDataOnly(const TArray<UObject*>& Objects) {
	this->ListViewPtr->SetListItems(Objects);
}

void UListObject::AddItem(UObject* Object) {
	this->ListViewPtr->AddItem(Object);
}

void UListObject::AddItems(TArray<UObject*> Objects) {	
	TArray<UObject*> Temp = this->GetItems();
	Temp.Append(Objects);
	this->ListViewPtr->SetListItems(Temp);
}

void UListObject::AddItemByIndex(UObject* Object, int32 Index) {

	if (Index < 0 || Index >= this->GetNums())
	{
		this->ListViewPtr->AddItem(Object);
		return;
	}

	TArray<UObject*> Temp = this->GetItems();
	Temp.EmplaceAt(Index, Object);
	this->ListViewPtr->SetListItems(Temp);
}

void UListObject::AddItemsByIndex(TArray<UObject*> Objects, int32 Index) {

	if (Index < 0 || Index >= this->GetNums())
	{
		this->AddItems(Objects);
		return;
	}

	int32 Count = 0;
	TArray<UObject*> Temp = this->GetItems();
	for (auto It = Objects.CreateIterator(); It; ++It)
	{
		Temp.Insert(*It, Index + Count);
		Count++;
	}
}

void UListObject::RemoveItemByObject(UObject* Object) {
	this->ListViewPtr->RemoveItem(Object);
}

void UListObject::RemoveItemByObjects(TArray<UObject*> Objects) {
	for (auto It = Objects.CreateIterator(); It; ++It)
	{
		this->ListViewPtr->RemoveItem(*It);
	}
}

void UListObject::RemoveItemByIndex(int32 Index) {
	this->ListViewPtr->RemoveItem(this->ListViewPtr->GetItemAt(Index));
}

void UListObject::RemoveItemByIndexs(TArray<int32> Indexs)
{
	for (auto It = Indexs.CreateIterator(); It; ++It)
	{
		this->ListViewPtr->RemoveItem(this->ListViewPtr->GetItemAt(*It));
	}
}

void UListObject::UpdateObject(UObject* Object) {
	if (this->ListViewPtr->IsItemVisible(Object))
	{
		this->ListViewPtr->RequestRefresh();
	}
}

void UListObject::UpdateObjects(TArray<UObject*> Object) {
	for (auto It = Object.CreateIterator(); It; ++It)
	{
		if (this->ListViewPtr->IsItemVisible(*It))
		{
			this->ListViewPtr->RequestRefresh();
			return;
		}
	}
}

UObject* UListObject::GetItem(int32 Index) {
	return this->ListViewPtr->GetItemAt(Index);
}

void UListObject::GetItemBySelectIndex(const TArray<int32>& Index, TArray<UObject*>& OutObjects) {
	for (auto It = Index.CreateConstIterator(); It; ++It)
	{
		if (UObject* Obj = this->ListViewPtr->GetItemAt(*It))
		{
			OutObjects.Emplace(Obj);
		}
	}
}

const TArray<UObject*>& UListObject::GetItems() const {
	return this->ListViewPtr->GetListItems();
}

int32 UListObject::GetNums() const {
	return this->ListViewPtr->GetNumItems();
}
