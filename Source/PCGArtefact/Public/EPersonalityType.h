#pragma once

#include "CoreMinimal.h"
#include "EPersonalityType.generated.h"

UENUM(BlueprintType)
enum class EPersonalityType : uint8
{
	Friendly,
	Flirty,
	Gloomy,
	Introverted,
	Extroverted,
	Relaxed,
	Crabby
};
