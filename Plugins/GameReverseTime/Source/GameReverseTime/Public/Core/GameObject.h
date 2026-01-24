#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/NoExportTypes.h"
#include "GameDataStaticObjectSpawnImp.h"
#include "GameObject.generated.h"

//静态游戏类声明
namespace GameDataStaticObjects {
	//静态对象池子
	extern GAMEREVERSETIME_API TMap<FName, class UGameDataStaticObject*> StaticPools;
	//生成器
	extern GAMEREVERSETIME_API TSharedPtr<class GameDataStaticObjectSpawn> Spawn;
}


/*
	用于检测UGameObject对象
	1.使用方法和TSubclassOf一致
	2.静态加载-阻塞
*/
template<typename ClassType, typename SoftClassType>
ClassType* TSubclassOf_StaticLoadObject(
	TSoftClassPtr<SoftClassType> FromPtr,
	const UObject* WorldContextObject,
	UObject* Outer = (UObject*)GetTransientPackage()
) {

#if WITH_EDITOR
	static_assert(std::is_base_of<UGameObject, ClassType>::value,
		"This structure cannot use this type ! Must be a derived class of UGameObject type"
		);
#endif

	UClass* LocalResource = FromPtr.Get();
	if (LocalResource == nullptr)
	{
		LocalResource = FromPtr.LoadSynchronous();
	}

	if (LocalResource)
	{
		ClassType* AssetObject = NewObject<ClassType>(Outer, LocalResource, NAME_None);
		AssetObject->SetWorldContext(WorldContextObject);
		return AssetObject;
	}
	return nullptr;
}

/*
	用于检测UGameObject对象
	1.使用方法和TSubclassOf一致
	2.静态加载-阻塞
*/
template<typename ClassType>
ClassType* TSubclassOf_StaticLoadObject(
	TSoftObjectPtr<ClassType> FromPtr,
	const UObject* WorldContextObject
) {

#if WITH_EDITOR
	static_assert(std::is_base_of<UGameObject, ClassType>::value,
		"This structure cannot use this type ! Must be a derived class of UGameObject type"
		);
#endif

	ClassType* LocalResource = FromPtr.Get();
	if (LocalResource == nullptr)
	{
		LocalResource = FromPtr.LoadSynchronous();
	}

	if (LocalResource)
	{
		LocalResource->SetWorldContext(WorldContextObject);
		return LocalResource;
	}
	return nullptr;
}



//游戏数据对象库函数
UCLASS()
class GAMEREVERSETIME_API UGameObjectLib : public UObject {

	GENERATED_BODY()

	friend class UGameObject;

public:

	//检测游戏数据对象是否是一个有效对象
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static bool IsValidGameObject(const UObject* WorldContextObject, UGameObject* GameObject);
};

/*
	游戏数据对象
	1.包含一个World对象
	2.可以拥有Outer对象
	3.蓝图可以使用
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEREVERSETIME_API UGameObject : public UObject {

	GENERATED_BODY()

	friend class UGameObjectLib;

public:

	//new 一个游戏数据对象
	template<typename ClassType>
	static ClassType* NewGameObject(const UObject* WorldContextObject, UObject* Outer = (UObject*)GetTransientPackage())
	{
#if WITH_EDITOR
		static_assert(std::is_base_of<UGameObject, ClassType>::value,
			"This class cannot use this type ! Must be a derived class of UGameObject type"
			);
#endif
		ClassType* TempObj = NewObject<ClassType>(Outer);
		TempObj->LocalWorld = WorldContextObject->GetWorld();
		TempObj->BeginPlay();
		return TempObj;
	}

	//新建一个游戏数据对象
	UFUNCTION(BlueprintCallable, Category = "GameObject", meta = (WorldContext = "WorldContextObject"))
	static UGameObject* NewGameObject(const UObject* WorldContextObject, TSoftClassPtr<UGameObject> GameObject, UObject* Outer);

	//新建一个游戏数据对象
	UFUNCTION(BlueprintCallable, Category = "GameObject", meta = (WorldContext = "WorldContextObject"))
	static UGameObject* NewGameObjectFromPath(const UObject* WorldContextObject, FSoftClassPath Path, UObject* Outer);

public:

	//开始函数
	UFUNCTION(BlueprintNativeEvent, Category = "GameObject")
	void BeginPlay();
	virtual void BeginPlay_Implementation();

	//获取Outer对象
	UFUNCTION(BlueprintPure, Category = "GameObject")
	FORCEINLINE UObject* GetOuterObject() { return this->GetOuter(); }

public:
	//获取当前世界
	virtual UWorld* GetWorld() const override;
#if WITH_EDITOR
	//让蓝图实现获取世界函数
	virtual bool ImplementsGetWorld() const override { return true; }
#endif
	//设置世界对象
	FORCEINLINE	void SetWorldContext(const UObject* WorldContextObject) {
#if WITH_EDITOR
		if (WorldContextObject) this->LocalWorld = WorldContextObject->GetWorld();
#else
		this->LocalWorld = WorldContextObject->GetWorld();
#endif
	}
protected:
	//当前的世界对象
	UPROPERTY()
	UWorld* LocalWorld = nullptr;
};

//UGameObject类型的构造函数模板
template<typename ClassType = UGameObject>
ClassType* NewGameObject(const UObject* WorldContextObject, UObject* Outer = (UObject*)GetTransientPackage()) {
	return UGameObject::NewGameObject<ClassType>(WorldContextObject, Outer);
}

/*
	静态游戏对象-类型静态单例
	1.可以直接通过内部函数获取数据子系统
	2.通过 GET_GAMESTATIC() 或者 蓝图 GetGameStatic() 即可获取对象
	3.统一结构，继承自UGameDataStaticObject就可以使用数据子系统
	4.自带编译期检查
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEREVERSETIME_API UGameDataStaticObject : public UGameObject {

	GENERATED_BODY()

	friend class UGameDataSubsystem;

	friend GameDataStaticObjects::GameDataStaticObjectSpawn;

public:

	//新建一个游戏数据对象
	UFUNCTION(BlueprintCallable, Category = "GameDataStaticObject", meta = (WorldContext = "WorldContextObject"))
	static UGameDataStaticObject* GetGameStatic(const UObject* WorldContextObject, TSoftClassPtr<UGameDataStaticObject> GameStaticObjectPtr);

	//新建一个游戏数据对象
	UFUNCTION(BlueprintCallable, Category = "GameDataStaticObject", meta = (WorldContext = "WorldContextObject"))
	static UGameDataStaticObject* GetGameStaticFromPath(const UObject* WorldContextObject, FSoftClassPath GameStaticObjectPath);

	//获取数据子系统
	UFUNCTION(BlueprintPure)
	class UGameDataSubsystem* GetSubsystem();

protected:
	//初始化
	virtual void Initialize();
	//卸载
	virtual void DeInitialize();
};

//全局唯一
namespace GameDataStaticObjects {

	//Imp
	//See GameObject.cpp


	//静态对象生成器
	class GAMEREVERSETIME_API GameDataStaticObjectSpawn {

	public:

		//获取静态对象的结构体
		template<typename GameDataStaticObjectType>
		struct GameDataStaticObject {
			/*
				see GlobalMacroFunction.h -> GET_GAMESTATIC(GameDataStaticObjectType,WorldContextObject);
			*/
			/*operator GameDataStaticObjectType* () {
				return GameDataStaticObject<GameDataStaticObjectType>::Get();
			}
			GameDataStaticObjectType* operator->() {
				return GameDataStaticObject<GameDataStaticObjectType>::Get();
			}*/

			//Cpp
			static GameDataStaticObjectType* Get(const class UObject* WorldContextObject, FName Name) {

#if WITH_EDITOR
				static_assert(std::is_base_of<UGameDataStaticObject, GameDataStaticObjectType>::value,
					"This structure cannot use this type ! Must be a derived class of UGameDataStaticObject type"
					);
#endif

				UGameDataStaticObject** Target = StaticPools.Find(Name);
				if (Target)
				{
					(*Target)->SetWorldContext(WorldContextObject);
					return Cast<GameDataStaticObjectType>((*Target));
				}

				GameDataStaticObjectType* Obj = UGameObject::NewGameObject<GameDataStaticObjectType>(WorldContextObject);
				Obj->AddToRoot();
				Obj->BeginPlay();
				StaticPools.Emplace(Name, Obj);
				return Obj;
			}

			//Bp
			static GameDataStaticObjectType* Get(const class UObject* WorldContextObject, const TSoftClassPtr<GameDataStaticObjectType>& Ptr) {

#if WITH_EDITOR
				if (Ptr.IsNull())
				{
					//未设置引用路径
					ensureMsgf(false, TEXT("TSoftClassPtr<UGameDataStaticObject> is null"));
					return nullptr;
				}
#endif
				UGameDataStaticObject** Target = StaticPools.Find(FName(*(Ptr.GetAssetName())));
				if (Target)
				{
					(*Target)->SetWorldContext(WorldContextObject);
					return (*Target);
				}

				GameDataStaticObjectType* BpObj = TSubclassOf_StaticLoadObject<GameDataStaticObjectType>(Ptr, WorldContextObject);
#if WITH_EDITOR
				if (!IsValid(BpObj))
				{
					//资源引用错误
					ensureMsgf(false, TEXT("TSoftClassPtr<UGameDataStaticObject> is nullptr"));
					return nullptr;
				}
#endif
				BpObj->AddToRoot();
				BpObj->BeginPlay();
				StaticPools.Emplace(FName(*(Ptr.GetAssetName())), BpObj);
				return BpObj;
			}
		};
	public:
		~GameDataStaticObjectSpawn();
		//回收
		void RecycleGarbage(FString Tip);
	};
}