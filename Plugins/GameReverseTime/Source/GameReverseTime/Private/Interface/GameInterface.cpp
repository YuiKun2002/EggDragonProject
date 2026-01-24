#include "Interface/GameInterface.h"
#include "Core/GameAssetSubsystem.h"

void UGameMain::GameLoad_Implementation(UGameAssetSubsystem* GameAssetSubsystem) {

}

bool UGameMain::Main_Implementation(UObject* CallObject) {
	return true;
}

UWorld* UGameMain::GetWorld() const {

	if (this->LocalWorld)
	{
		return this->LocalWorld;
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

