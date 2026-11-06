#include "ANPCCharacter.h"

ANPCCharacter::ANPCCharacter(): NPCState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANPCCharacter::InitialiseFromState(const FNPCState& NewState)
{
	NPCState = NewState;
	NPC_ID = NewState.ID;
	NPCState.UpdateAllEnums();
}

int32 ANPCCharacter::GetNPCID_Implementation() const
{
	return NPC_ID;
}

const FNPCState ANPCCharacter::GetNPCState_Implementation() const
{
	return NPCState;
}

void ANPCCharacter::SetNPCState_Implementation(FNPCState NewState)
{
	NPCState = NewState;
}





