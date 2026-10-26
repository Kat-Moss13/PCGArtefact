// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ANPCCharacter.h"
#include "INPCInterface.h"
#include "UObject/Interface.h"
#include "INPCPopulationManagerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UNPCPopulationManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PCGARTEFACT_API INPCPopulationManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
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
