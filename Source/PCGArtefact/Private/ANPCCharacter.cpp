// Fill out your copyright notice in the Description page of Project Settings.


#include "ANPCCharacter.h"
#include "UNPCRegistry.h"

ANPCCharacter::ANPCCharacter(): NPCState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANPCCharacter::InitialiseFromState(const FNPCState& NewState)
{
	NPCState = NewState;

	NPCState.UpdateAllEnums();
}




