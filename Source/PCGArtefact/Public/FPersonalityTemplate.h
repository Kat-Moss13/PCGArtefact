#pragma once

#include "CoreMinimal.h"
#include "EPersonalityType.h"
#include "FPersonalityTemplate.generated.h"

USTRUCT(BlueprintType)
struct FPersonalityTemplate : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPersonalityType PersonalityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoodModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SocialModifier;
};
