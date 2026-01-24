#pragma once

#include "CoreMinimal.h"
#include "Interface/GameInterface.h"
#include "GameLoad.generated.h"

extern GAMEREVERSETIME_API const FString GAMELOAD;

/**
 * mod被加载时，执行
 */
UCLASS()
class GAMEREVERSETIME_API UGameLoad : public UGameMain
{
	GENERATED_BODY()

public:
	virtual FString GetExecTag() const { return GAMELOAD; }
};
