// Fill out your copyright notice in the Description page of Project Settings.


#include "ANPCSpawner.h"

#include "ANPCCharacter.h"
#include "UNPCIDGenerator.h"
#include "UNPCRegistry.h"


ANPCSpawner::ANPCSpawner(): Registry(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

ANPCCharacter* ANPCSpawner::SpawnNPCFromPCG(const FVector& location, const FRotator& rotation,
                                            const FNPCState& initialState)
{
	if(!NPCClass) return nullptr;
	
	Registry = GetWorld()->GetGameInstance()->GetSubsystem<UNPCRegistry>();
	if (!Registry) return nullptr;
	
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	ANPCCharacter* NPC = GetWorld()->SpawnActor<ANPCCharacter>(NPCClass, location, rotation, spawnParameters);
	if (!NPC) return nullptr;
	
	int NewID = Registry->IDGenerator->GenerateID();
	
	FNPCState StateCopy = initialState;
	StateCopy.ID = NewID;
	StateCopy.UpdateAllEnums();
	
	NPC->InitialiseFromState(StateCopy);
	
	if (Registry)
	{
		Registry->RegisterNPC(NewID, NPC, StateCopy);
	}

	return NPC;
}


