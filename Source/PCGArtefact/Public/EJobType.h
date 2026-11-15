#pragma once

#include "CoreMinimal.h"
#include "EJobType.generated.h"

UENUM(BlueprintType)
enum class EJobType : uint8
{
	Farmer,
	ShopKeeper,
	Unemployed,
	Baker,
	Librarian,
	Toolsmith
};
