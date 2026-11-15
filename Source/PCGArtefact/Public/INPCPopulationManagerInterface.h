#pragma once

#include "CoreMinimal.h"
#include "ANPCCharacter.h"
#include "UObject/Interface.h"
#include "INPCPopulationManagerInterface.generated.h"

UINTERFACE()
class UNPCPopulationManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class PCGARTEFACT_API INPCPopulationManagerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RegisterNPCs();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetNPCCount();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ANPCCharacter* GetNPCByID(int32 NPC_ID);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<ANPCCharacter*> GetNPCs();
};
