#pragma once

#include "CoreMinimal.h"
#include "FNPCNamePool.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FNPCNamePool : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FirstName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Surname;
};
