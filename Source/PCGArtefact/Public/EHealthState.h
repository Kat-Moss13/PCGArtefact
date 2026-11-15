#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EHealthState : uint8
{
	Healthy,
	Injured,
	Sick,
	Critical
};
