#include "Core/DataTableAssetData.h"
#include "Core/GameDataSubsystem.h"
#include "Core/GameAssetSubsystem.h"

UDataTable* InitDataTable_Imp(const UObject* WorldContextObject, FName&& Name1, FName&& Name2, const FString& PackageID)
{
	return UGameDataSubsystem::GetGameDataSubsystemStatic(WorldContextObject)->GetGamePackDataAsset(PackageID)->
		GetDataByName_Imp(MoveTemp(Name1))->GetDataByName_Imp(MoveTemp(Name2));
}

TSharedPtr<DataTablePackageObject> GetDataTablePackageObject_Imp(const UObject* WorldContextObject, FName&& Name1, FName&& Name2, const FString& PackageID)
{
	return UGameDataSubsystem::GetGameDataSubsystemStatic(WorldContextObject)->
		GetPackage(PackageID, MoveTemp(Name1), MoveTemp(Name2));
}

TSharedPtr<DataTablePackageObject> GetDataTablePackageObject_Imp2(const UObject* WorldContextObject, FName Name1, FName Name2, const FString& PackageID)
{
	FName A = Name1;
	FName B = Name2;
	return UGameDataSubsystem::GetGameDataSubsystemStatic(WorldContextObject)->
		GetPackage(PackageID, MoveTemp(A), MoveTemp(B));
}

int32 GetDynamicID(const UObject* WorldContextObject)
{
	UGameAssetSubsystem* As = UGameAssetSubsystem::GetGameAssetSubsystemStatic(WorldContextObject);

	return As->GetDynamicID();
}

void UpdateDynamicID(const UObject* WorldContextObject, int32 UpdateTargetID)
{
	UGameAssetSubsystem* As = UGameAssetSubsystem::GetGameAssetSubsystemStatic(WorldContextObject);
	As->SetDynamicID(UpdateTargetID);
}

DataTablePackageObject::DataTablePackageObject(const FString& StrPackageID, const FString& StrTableAssetName, const FString& StrTableName, UDataTable* DataTablePtr)
{
	this->Data.TablePackageID = StrPackageID;
	this->Data.TableAssetName = StrTableAssetName;
	this->Data.TableName = StrTableName;
	this->Data.DataTableInstance = DataTablePtr;
}

UDataTablePackageObject* UDataTablePackageObject::BP_MakeDataTablePackageObject(UGameDataSubsystem* GameDataSubsystem, const FString& PackageID, TSubclassOf<class UAssetCategoryName> CategoryName, TSubclassOf<class UAssetCategoryName> Name, TSoftObjectPtr<class UDataTable> TablePtr)
{
	UGameObjectLib::IsValidGameObject(GameDataSubsystem, CategoryName.GetDefaultObject());
	UGameObjectLib::IsValidGameObject(GameDataSubsystem, Name.GetDefaultObject());
	return UDataTablePackageObject::MakeDataTablePackageObject(GameDataSubsystem, PackageID, CategoryName.GetDefaultObject()->GetCategoryName(), Name.GetDefaultObject()->GetCategoryName(), TablePtr);
}

UDataTablePackageObject* UDataTablePackageObject::MakeDataTablePackageObject(UGameDataSubsystem* GameDataSubsystem, const FString& PackageID, FName CategoryName, FName Name, TSoftObjectPtr<UDataTable> TablePtr)
{
	if (GameDataSubsystem)
	{
		UDataTablePackageObject* PakObj = NewObject<UDataTablePackageObject>();
		PakObj->DataTablePackageObjectPtr = GameDataSubsystem->GetPackage(PackageID, CategoryName, Name);
		//如果数据无效，表示此数据表是不属于管理器的，自定义数据包构建
		if (!PakObj->DataTablePackageObjectPtr.IsValid())
		{
			PakObj->DataTablePackageObjectPtr = MakeShareable(
				new DataTablePackageObject(
					PackageID,
					CategoryName.ToString(),
					Name.ToString(),
					TablePtr.LoadSynchronous()
				)
			);

			TMap<FString, TSharedPtr<class DataTablePackageObject>>* Maps = GameDataSubsystem->GetPackages().Find(PackageID);
			if (Maps)
			{
				Maps->Emplace(PakObj->DataTablePackageObjectPtr->GetGlobalPackageID(), PakObj->DataTablePackageObjectPtr);
			}
			else {
				TMap<FString, TSharedPtr<class DataTablePackageObject>> NewPak;
				NewPak.Emplace(PakObj->DataTablePackageObjectPtr->GetGlobalPackageID(), PakObj->DataTablePackageObjectPtr);
				GameDataSubsystem->GetPackages().Emplace(PackageID, NewPak);
			}

		}
		return PakObj;
	}

	return NewObject<UDataTablePackageObject>();
}

bool UDataTablePackageObject::IsValid() {
	if (this->DataTablePackageObjectPtr.IsValid())
	{
		return  this->DataTablePackageObjectPtr->IsValid();
	}
	return false;
}
