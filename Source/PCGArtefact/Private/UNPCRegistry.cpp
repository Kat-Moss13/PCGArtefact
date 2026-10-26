// Fill out your copyright notice in the Description page of Project Settings.


#include "UNPCRegistry.h"
#include "ANPCCharacter.h"
#include "UNPCIDGenerator.h"

void UNPCRegistry::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UNPCRegistry::RegisterNPC(const int32 NPC_ID, ANPCCharacter* NPCActor, const FNPCState& State)
{
	NPCActors.Add(NPC_ID, NPCActor);
	NPCStates.Add(NPC_ID, State);
}

void UNPCRegistry::UnregisterNPC(const int32 NPC_ID)
{
	NPCActors.Remove(NPC_ID);
	NPCStates.Remove(NPC_ID);
}

ANPCCharacter* const* UNPCRegistry::GetNPCActor(const int32 NPC_ID) const
{
	if(ANPCCharacter* const* Found = NPCActors.Find(NPC_ID))
	{
		return Found;
	}
	return nullptr;
}

const FNPCState* UNPCRegistry::GetNPCState(const int32 NPC_ID) const { return NPCStates.Find(NPC_ID);}

const TMap<int32, ANPCCharacter*>& UNPCRegistry::GetAllNPCs() const { return NPCActors; }
