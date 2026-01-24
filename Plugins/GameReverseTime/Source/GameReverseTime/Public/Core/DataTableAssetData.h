// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataTableAssetData.generated.h"

class UDataTable;

//合并ID，资产分类，资产名称
#define ConvertGlobalPackageID(PakID,CtName,Name) FString::Printf(TEXT("%s.%s.%s"),*PakID,*CtName,*Name)
//增加新增ID项目
#define AppendGlobalPackageItemID(Name) FString::Printf(TEXT(".%s"),*Name)


//初始化数据表
GAMEREVERSETIME_API UDataTable* InitDataTable_Imp(const UObject* WorldContextObject, FName&& Name1, FName&& Name2, const FString& PackageID);
//获取包对象
GAMEREVERSETIME_API TSharedPtr<class DataTablePackageObject> GetDataTablePackageObject_Imp(const UObject* WorldContextObject, FName&& Name1, FName&& Name2, const FString& PackageID);
GAMEREVERSETIME_API TSharedPtr<class DataTablePackageObject> GetDataTablePackageObject_Imp2(const UObject* WorldContextObject, FName Name1, FName Name2, const FString& PackageID);

//获取动态ID
GAMEREVERSETIME_API int32 GetDynamicID(const UObject* WorldContextObject);
//将动态ID更新至目标ID
GAMEREVERSETIME_API void UpdateDynamicID(const UObject* WorldContextObject, int32 UpdateTargetID);

/*
	数据表格包
	1.包含一个数据表对象
	2.包含包的信息
*/
USTRUCT(BlueprintType)
struct GAMEREVERSETIME_API FDataTablePackage
{
	GENERATED_USTRUCT_BODY()

	//数据表实例
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* DataTableInstance = nullptr;

	//包ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TablePackageID;

	//表资产ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TableAssetName;

	//表名称
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TableName;

};

/*
	数据表包自定义属性类
	1.此类将跟随数据包类
*/
class GAMEREVERSETIME_API DataTablePackageCustomProperty {};

/*
	数据表包类
	1.管理一个数据表资产
	2.含有表的来源信息
*/
class GAMEREVERSETIME_API DataTablePackageObject {

public:

	DataTablePackageObject() {};

	DataTablePackageObject(const FString& StrPackageID, const FString& StrTableAssetName, const FString& StrTableName, UDataTable* DataTablePtr);

	~DataTablePackageObject() {
		this->Property.Reset();
		this->Property = nullptr;
		this->Data.DataTableInstance = nullptr;
	}

	//获取数据包属性类
	template<class PropertyType = DataTablePackageCustomProperty>
	TSharedPtr<PropertyType> GetPackageProperty() {
		if (this->Property.IsValid())
		{
			return StaticCastSharedPtr<PropertyType>(this->Property);
		}
		else {
			this->Property = MakeShareable<PropertyType>(new PropertyType());
		}
		return StaticCastSharedPtr<PropertyType>(this->Property);
	}

	//是否有效
	FORCEINLINE bool IsValid() const {
		return this->Data.DataTableInstance != nullptr;
	}

	//是否存在
	FORCEINLINE bool Find(const FName& Name) const {
		return this->Data.DataTableInstance->GetRowMap().Contains(Name);
	}

	//获取名称
	FORCEINLINE void GetNames(TArray<FName>& Names) {
		this->Data.DataTableInstance->GetRowMap().GetKeys(Names);
	}

	//获取数量
	FORCEINLINE int32 GetNum() const {
		return this->Data.DataTableInstance->GetRowMap().Num();
	}

	//获取包ID
	FORCEINLINE FString GetPackageID() const {
		return this->Data.TablePackageID;
	}

	//获取表资产名称
	FORCEINLINE FString GetDataTableAssetName() const {
		return this->Data.TableAssetName;
	}

	//获取表名称
	FORCEINLINE FString GetDataTableName() const {
		return this->Data.TableName;
	}

	//获取数据包
	FORCEINLINE FDataTablePackage GetPackage() const {
		return this->Data;
	}

	//获取源表格
	FORCEINLINE const TMap<FName, uint8*>& GetSourceDataTable() {
		return this->Data.DataTableInstance->GetRowMap();
	}

	//获取全局ID名称
	FORCEINLINE FString GetGlobalPackageID() {
		return ConvertGlobalPackageID(this->Data.TablePackageID, this->Data.TableAssetName, this->Data.TableName);
	}

	//获取数据表
	FORCEINLINE class UDataTable* GetDataTable() {
		return this->Data.DataTableInstance;
	}

	//获取行名称
	void GetRowNames(TArray<FName>& Names) {
		Names.Reserve(Names.Num() + this->Data.DataTableInstance->GetRowMap().Num());
		for (
			TMap<FName, uint8*>::TConstIterator RowMapIter(
				this->Data.DataTableInstance->GetRowMap().CreateConstIterator());
			RowMapIter;
			++RowMapIter
			)
		{
			Names.Add(RowMapIter.Key());
		}
	}

	//获取数据
	template<typename ValueType>
	void GetDatas(TArray<ValueType>& OutValues) {
		OutValues.Reserve(OutValues.Num() + this->Data.DataTableInstance->GetRowMap().Num());
		for (
			TMap<FName, uint8*>::TConstIterator RowMapIter(
				this->Data.DataTableInstance->GetRowMap().CreateConstIterator());
			RowMapIter;
			++RowMapIter
			)
		{
			OutValues.Add(
				*(reinterpret_cast<ValueType*>(
					RowMapIter.Value()
					)));
		}
	}

	template<typename ValueType>
	ValueType GetDataByName(const FName& Name)
	{
		uint8* const* TempData = this->Data.DataTableInstance->GetRowMap().Find(Name);
		if (TempData)
		{
			return *(reinterpret_cast<ValueType*>(
				*TempData
				));
		}

		ValueType NullValue;
		return NullValue;
	}

	template<typename ValueType>
	FORCEINLINE void GetDataByName(const FName& Name, ValueType& OutData) {
		OutData = this->GetDataByName<ValueType>(Name);
	}

	template<typename ValueType>
	FORCEINLINE ValueType* Find(const FName& Name) {
		return reinterpret_cast<ValueType*>(*(this->Data.DataTableInstance->GetRowMap().Find(Name)));
	}

	template<typename ValueType>
	FORCEINLINE const ValueType* Find(const FName& Name) const {
		return (reinterpret_cast<const ValueType*>(*(this->Data.DataTableInstance->GetRowMap().Find(Name))));
	}

protected:

	FDataTablePackage Data;

	TSharedPtr<DataTablePackageCustomProperty> Property;

};


//数据表包对象
UCLASS(BlueprintType, Blueprintable)
class GAMEREVERSETIME_API UDataTablePackageObject : public UObject {

	GENERATED_BODY()

public:

	//生成一个数据表包对象
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MakeDataTablePackageObject"))
	static UDataTablePackageObject* BP_MakeDataTablePackageObject(
		class UGameDataSubsystem* GameDataSubsystem,
		const FString& PackageID,
		TSubclassOf<class UAssetCategoryName> CategoryName,
		TSubclassOf<class UAssetCategoryName> Name,
		TSoftObjectPtr<class UDataTable> TablePtr
	);

	static UDataTablePackageObject* MakeDataTablePackageObject(
		class UGameDataSubsystem* GameDataSubsystem,
		const FString& PackageID,
		FName CategoryName,
		FName Name,
		TSoftObjectPtr<class UDataTable> TablePtr
	);

public:

	//是否有效
	UFUNCTION(BlueprintPure)
	bool IsValid();

	//获取数据表
	UFUNCTION(BlueprintPure)
	class UDataTable* GetDataTable() { return this->DataTablePackageObjectPtr->GetDataTable(); }

	FORCEINLINE TSharedPtr<DataTablePackageObject>& GetDataTablePackageObject() { return this->DataTablePackageObjectPtr; }

protected:

	TSharedPtr<DataTablePackageObject> DataTablePackageObjectPtr;

};


//数据表获取
template<class DataTableType>
class DataTableAssetData
{
public:
	DataTableAssetData(UDataTable* Data);
	DataTableAssetData() {};
public:
	//初始化数据表
	void InitDataTable(UDataTable* Data);
	//获取数据表
	const TMap<FName, DataTableType>& GetDatas();
	//获取值
	DataTableType GetDataByName(FName Name);
	void GetDataByName(FName Name, DataTableType& OutData);
	//获取Name数组
	void GetNames(TArray<FName>& Names);
	//获取值
	void GetValues(TArray<DataTableType>& Values);
	//检查行是否存在
	bool CheckRowIsValid(FName Name) { return this->SourceDatas.Contains(Name); }
	//获取数据表
	TMap<FName, DataTableType> GetDataTable() const { return this->SourceDatas; }
public:
	//新增
	bool AddTemp(FName Name, const DataTableType& Data);
	//查询
	bool Find(FName Name);
public:
	//获取数量
	int32 Num() const;
protected:
	TMap<FName, DataTableType> SourceDatas;
private:
	void InitData(UDataTable* DataTable, TMap<FName, DataTableType>& OutDatas);
};

template<class DataTableType>
bool DataTableAssetData<DataTableType>::Find(FName Name)
{
	return (SourceDatas.Find(Name) != nullptr);
}

template<class DataTableType>
bool DataTableAssetData<DataTableType>::AddTemp(FName Name, const DataTableType& Data)
{
	if (this->Find(Name))
	{
		return false;
	}

	this->SourceDatas.Emplace(Name, Data);
}


template<class DataTableType>
DataTableAssetData<DataTableType>::DataTableAssetData(
	UDataTable* Data
)
{
	if (IsValid(Data))
	{
		this->InitDataTable(Data);
	}
}

template<class DataTableType>
void DataTableAssetData<DataTableType>::InitDataTable(UDataTable* Data)
{
	if (IsValid(Data))
	{
		if (!this->SourceDatas.Num())
		{
			this->InitData(Data, this->SourceDatas);
		}
	}
	else {
		SourceDatas.Empty();
	}
}

template<class DataTableType>
void DataTableAssetData<DataTableType>::InitData(UDataTable* DataTable, TMap<FName, DataTableType>& OutDatas)
{
	OutDatas.Reserve(OutDatas.Num() + DataTable->GetRowMap().Num());
	for (
		TMap<FName, uint8*>::TConstIterator RowMapIter(DataTable->GetRowMap().CreateConstIterator());
		RowMapIter;
		++RowMapIter
		)
	{
		OutDatas.Add(
			RowMapIter.Key(),
			*(reinterpret_cast<DataTableType*>(
				RowMapIter.Value()
				)));
	}
}

template<class DataTableType>
const TMap<FName, DataTableType>& DataTableAssetData<DataTableType>::GetDatas()
{
	return this->SourceDatas;
}

template<class DataTableType>
void DataTableAssetData<DataTableType>::GetDataByName(FName Name, DataTableType& OutData)
{
	OutData = this->GetDataByName(Name);
}

template<class DataTableType>
DataTableType DataTableAssetData<DataTableType>::GetDataByName(FName Name)
{
	DataTableType* Target = this->SourceDatas.Find(Name);
	if (Target)
	{
		return *Target;
	}

	UE_LOG(LogTemp, Error, TEXT("[%s]%s：未找到Row？"), __FUNCTION__, *Name.ToString());

	return DataTableType();
}

template<class DataTableType>
void DataTableAssetData<DataTableType>::GetValues(TArray<DataTableType>& Values)
{
	this->SourceDatas.GenerateValueArray(Values);
}

template<class DataTableType>
void DataTableAssetData<DataTableType>::GetNames(TArray<FName>& Names)
{
	this->SourceDatas.GenerateKeyArray(Names);
}

template<class DataTableType>
int32 DataTableAssetData<DataTableType>::Num() const
{
	return this->SourceDatas.Num();
}

/*
	填充数据,用于填充物品类型的数据表，带有ID结构的结构体
	1.类型必须继承自 FItemBaseStructData
*/
template<typename ValueType>
void FillDataTable_Item(
	const UObject* WorldContextObject,
	TMap<int32, ValueType>& InData,
	DataTableAssetData<ValueType>& InTable,
	FName&& DataName,
	FName&& Name,
	const FString& PackageID
) {
	if (InData.Num() == 0)
	{
		int32 DyID = GetDynamicID(WorldContextObject);

		InTable.InitDataTable(InitDataTable_Imp(WorldContextObject, MoveTemp(DataName), MoveTemp(Name), PackageID));

		TArray<ValueType> Values;
		InTable.GetValues(Values);

		for (ValueType Data : Values)
		{
			Data.SetID(DyID);
			InData.Emplace(DyID, Data);
			++DyID;
		}

		UpdateDynamicID(WorldContextObject, DyID);

		InTable.InitDataTable(nullptr);
	}
}

//初始化数据表
template<typename ValueType>
void FillDataTable(
	const UObject* WorldContextObject,
	TMap<FName, ValueType>& InData,
	DataTableAssetData<ValueType>& InTable,
	FName&& DataName,
	FName&& Name,
	const FString& PackageID
) {
	if (InData.Num() == 0)
	{
		InTable.InitDataTable(InitDataTable_Imp(WorldContextObject, MoveTemp(DataName), MoveTemp(Name), PackageID));

		InData.Append(InTable.GetDatas());

		InTable.InitDataTable(nullptr);
	}
}

//获取数据表原生数据
template<typename Type>
TArray<Type>& GetDataTableSourceData(
	const UObject* WorldContextObject,
	TArray<Type>& InData,
	DataTableAssetData<Type>& InTable,
	FName&& DataName,
	FName&& Name,
	const FString& PackageID
) {
	if (InData.Num())
	{
		return InData;
	}
	else {

		InTable.InitDataTable(InitDataTable_Imp(WorldContextObject, MoveTemp(DataName), MoveTemp(Name), PackageID));

		InTable.GetValues(InData);

		InTable.InitDataTable(nullptr);
	}
	return InData;
}