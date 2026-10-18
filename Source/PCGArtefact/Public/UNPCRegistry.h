// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ANPCCharacter.h"
#include "FNPCState.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UNPCRegistry.generated.h"

class UNPCIDGenerator;
class ANPCCharacter;
/**
 * 
 */
UCLASS()
class PCGARTEFACT_API UNPCRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void RegisterNPC(int NPC_ID, ANPCCharacter* NPCActor, const FNPCState& State);
	void UnregisterNPC(int NPC_ID);
	ANPCCharacter* const* GetNPCActor(int NPC_ID) const;
	const FNPCState* GetNPCState(int NPC_ID) const;
	const TMap<int, ANPCCharacter*>& GetAllNPCs() const;

	UPROPERTY()
	UNPCIDGenerator* IDGenerator;

private:
	
	UPROPERTY()
	TMap<int, ANPCCharacter*> NPCActors;
	
	UPROPERTY()
	TMap<int, FNPCState> NPCStates;

	
};


