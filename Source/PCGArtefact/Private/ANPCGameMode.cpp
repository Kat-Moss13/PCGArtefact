#include "ANPCGameMode.h"
#include "UTimeManagementComponent.h"

ANPCGameMode::ANPCGameMode()
{
	TimeManagement = CreateDefaultSubobject<UTimeManagementComponent>(
		TEXT("TimeManagement"));
}
