//  
//  

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ListObject.generated.h"

/**
 * 列表对象
 */
UCLASS(BlueprintType,Blueprintable)
class GAMEREVERSETIME_API UListObject : public UObject
{
	GENERATED_BODY()

public:

	//生成一个列表对象
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	static UListObject* MakeListObject(class UListView* View, const TArray<UObject*>& Objects);

public:

	//刷新数据
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void ForceRefresh();

	//绑定数据
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void ResetBindData(const TArray<UObject*>& Objects, class UListView* View);

	//绑定数据
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void ResetBindDataOnly(const TArray<UObject*>& Objects);

	//新增物品
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void AddItem(UObject* Object);
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void AddItems(TArray<UObject*> Objects);

	//新增物品到指定索引位置
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void AddItemByIndex(UObject* Object,int32 Index);
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void AddItemsByIndex(TArray<UObject*> Objects,int32 Index);

	//移除物品
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void RemoveItemByObject(UObject* Object);
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void RemoveItemByObjects(TArray<UObject*> Objects);

	//移除物品
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void RemoveItemByIndex(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void RemoveItemByIndexs(TArray<int32> Indexs);

	//更新物品
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void UpdateObject(UObject* Object);
	//更新物品
	UFUNCTION(BlueprintCallable, Category = "ListObject")
	void UpdateObjects(TArray<UObject*> Object);

	//获取物品
	UFUNCTION(BlueprintPure, Category = "ListObject")
	UObject* GetItem(int32 Index);
	//获取指定索引的物品
	UFUNCTION(BlueprintPure, Category = "ListObject")
	void GetItemBySelectIndex(const TArray<int32>& Index, TArray<UObject*>& OutObjects);
	//获取所有物品
	UFUNCTION(BlueprintPure, Category = "ListObject")
	const TArray<UObject*>& GetItems() const;

	//获取数量
	UFUNCTION(BlueprintPure, Category = "ListObject")
	int32 GetNums() const;

private:

	UPROPERTY()
	TObjectPtr<UListView> ListViewPtr;
};
