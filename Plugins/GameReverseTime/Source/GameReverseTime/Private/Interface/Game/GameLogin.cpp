#include "Interface/Game/GameLogin.h"

const FString GAMELOGIN = TEXT("GameLogin_Exec");

bool UGameLogin::Main_Implementation(UObject* CallObject) {
	IGameLoginInterface* Inter = (IGameLoginInterface*)(CallObject);
	if (IsValid(CallObject) && Inter)
	{
		this->LoginInterface.SetInterface(Inter);
		this->LoginInterface.SetObject(CallObject);
		this->GameLoginInit();
		return true;
	}
	return false;
}
void UGameLogin::GameLoginInit_Implementation() {}
