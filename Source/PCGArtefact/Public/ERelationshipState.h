#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ERelationshipState : uint8
{
	Married,
	Love,
	Friends,
	Neutral,
	Dislike,
	Hate
};
