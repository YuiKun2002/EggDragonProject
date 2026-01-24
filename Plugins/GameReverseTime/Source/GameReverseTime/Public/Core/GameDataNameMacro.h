#pragma once
#include "CoreMinimal.h"
#include "JsonBlueprintFunctionLibrary.h"

typedef FName GameDataCategoryName;
typedef FName GameDataName;

//声明分类名称
#define DEF_CATEGORYNAME(CategoryName)\
static GameDataCategoryName CategoryName(TEXT(#CategoryName))\
//声明数据名称
#define DEF_NAME(Name)\
static GameDataName Name(TEXT(#Name))\

//获取UI分类名称
#define GET_UI_CATEGORYNAME(CategoryName)\
GameDataNameAsset::UI_##CategoryName::CategoryName\
//获取分类中的数据名称
#define GET_UI_NAME(CategoryName,Name)\
GameDataNameAsset::UI_##CategoryName::Name\
//获取分类名称
#define GET_DEF_CATEGORYNAME(CategoryName)\
GameDataNameAsset::GameDataAsset_GlobalAsset_##CategoryName::GlobalAsset_##CategoryName\
//获取分类中的数据名称
#define GET_DEF_NAME(CategoryName,Name)\
GameDataNameAsset::GameDataAsset_GlobalAsset_##CategoryName::Name\

//获取移动分类名称
#define GET_MOVE_CATEGORYNAME(CategoryName)\
MoveTemp(GET_DEF_CATEGORYNAME(CategoryName))\
//获取移动分类中的数据名称
#define GET_MOVE_NAME(CategoryName,Name)\
MoveTemp(GET_DEF_NAME(CategoryName,Name))\
