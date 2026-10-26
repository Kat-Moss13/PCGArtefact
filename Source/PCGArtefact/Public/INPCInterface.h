
#pragma once

#include "CoreMinimal.h"
#include "FNPCState.h"
#include "UObject/Interface.h"
#include "INPCInterface.generated.h"

UINTERFACE()
class UNPCInterface : public UInterface
{
	GENERATED_BODY()
};

class PCGARTEFACT_API INPCInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NPC")
	int32 GetNPCID() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NPC")
	const FNPCState GetNPCState() const;
};
