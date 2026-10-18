// Fill out your copyright notice in the Description page of Project Settings.


#include "ANPCCharacter.h"
#include "UNPCRegistry.h"

ANPCCharacter::ANPCCharacter(): NPCState(), Registry(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANPCCharacter::InitialiseFromState(const FNPCState& newState)
{
	NPCState = newState;

	NPCState.UpdateAllEnums();
}




