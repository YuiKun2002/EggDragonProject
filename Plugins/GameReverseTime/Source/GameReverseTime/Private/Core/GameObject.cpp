
#include "Core/GameObject.h"
#include "Misc/AssertionMacros.h"
#include "Core/GameDataSubsystem.h"

bool UGameObjectLib::IsValidGameObject(const UObject* WorldContextObject, UGameObject* GameObject) {
	if (IsValid(GameObject))
	{
		if (IsValid(WorldContextObject) && IsValid(WorldContextObject->GetWorld()))
		{
			GameObject->LocalWorld = WorldContextObject->GetWorld();
			return true;
		}
		else {
			return IsValid(GameObject->LocalWorld);
		}
	}
	return false;
}

UGameObject* UGameObject::NewGameObject(const UObject* WorldContextObject, TSoftClassPtr<UGameObject> GameObject, UObject* Outer) {

	UGameObject* BeginDataObject = TSubclassOf_StaticLoadObject<UGameObject>(GameObject, WorldContextObject, (Outer == nullptr ? (UObject*)GetTransientPackage() : Outer));
	BeginDataObject->BeginPlay();
	return BeginDataObject;
}

UGameObject* UGameObject::NewGameObjectFromPath(const UObject* WorldContextObject, FSoftClassPath Path, UObject* Outer) {

	UGameObject* BeginDataObject = TSubclassOf_StaticLoadObject<UGameObject>(
		TSoftClassPtr<UGameObject>(Path), WorldContextObject, (Outer == nullptr ? (UObject*)GetTransientPackage() : Outer)
	);
	BeginDataObject->BeginPlay();
	return BeginDataObject;
}

void UGameObject::BeginPlay_Implementation() {}

UWorld* UGameObject::GetWorld() const {

	if (this->LocalWorld)
	{
		return this->LocalWorld;
	}
	else {

		UE_LOG(LogTemp, Error, TEXT("LocalWorld is nullptr!"))
	}

	if (UObject* Outer = this->GetOuter())
	{
		if (!this->HasAnyFlags(RF_ClassDefaultObject)
			&& !Outer->HasAnyFlags(RF_BeginDestroyed)
			&& !Outer->IsUnreachable())
		{
			return Outer->GetWorld();
		}
	}

	UWorld* world = nullptr;
	if (UObject* Outer = this->GetOuter())
	{
		world = Outer->GetWorld();
	}

	if (world == nullptr && GEngine)
	{
		for (auto context : GEngine->GetWorldContexts())
		{
			if (context.WorldType != EWorldType::Editor || context.WorldType != EWorldType::EditorPreview)
			{
				world = context.World();
			}
		}
	}
	if (world == nullptr && GEngine)
	{
		FWorldContext* worldContext = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		if (worldContext) {
			return worldContext->World();
		}
	}
	return world;
}

//初始化
void UGameDataStaticObject::Initialize() {
#if WITH_EDITOR
	//GAME_LOG(__FUNCTION__, TEXT("游戏对象初始化"), {
	//	GAME_LOG_LINE(this,TEXT("Ptr：%p"), this);
	//	});
#endif

}
//卸载
void UGameDataStaticObject::DeInitialize() {
#if WITH_EDITOR
	//GAME_LOG(__FUNCTION__, TEXT("游戏对象卸载"), {
	//	GAME_LOG_LINE(this,TEXT("Ptr：%p"), this);
	//	});
#endif

}

UGameDataStaticObject* UGameDataStaticObject::GetGameStatic(const UObject* WorldContextObject, TSoftClassPtr<UGameDataStaticObject> GameStaticObjectPtr) {
	return GameDataStaticObjects::GameDataStaticObjectSpawn::GameDataStaticObject<UGameDataStaticObject>::Get(WorldContextObject, GameStaticObjectPtr);
}

UGameDataStaticObject* UGameDataStaticObject::GetGameStaticFromPath(const UObject* WorldContextObject, FSoftClassPath GameStaticObjectPath) {
	return GameDataStaticObjects::GameDataStaticObjectSpawn::GameDataStaticObject<UGameDataStaticObject>::Get(WorldContextObject, TSoftClassPtr<UGameDataStaticObject>(GameStaticObjectPath));
}


UGameDataSubsystem* UGameDataStaticObject::GetSubsystem() {
	return UGameDataSubsystem::GetGameDataSubsystemStatic(this);
}
