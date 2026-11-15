#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UWeightedRandomLibrary.generated.h"

USTRUCT(BlueprintType)
struct FWeightedRange
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Min = 0.0f;

	UPROPERTY(EditAnywhere)
	float Max = 0.0f;

	UPROPERTY(EditAnywhere)
	float Weight = 0.0f;
};

UCLASS()
class PCGARTEFACT_API UWeightedRandomLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category="RandomWithWeight")
	static float GetWeightedRandomFloat(const TArray<FWeightedRange>& Ranges, FRandomStream& RandomStream);
};
