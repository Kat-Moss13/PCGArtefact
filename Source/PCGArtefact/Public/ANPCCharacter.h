// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FNPCState.h"
#include "GameFramework/Character.h"
#include "ANPCCharacter.generated.h"

class UNPCRegistry;

UCLASS()
class PCGARTEFACT_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCCharacter();

	void InitialiseFromState(const FNPCState& newState);
	const FNPCState& GetState() const {return NPCState;}
	int GetID() const {return NPC_ID;}

	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	int NPC_ID = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	FNPCState NPCState;
	
	UPROPERTY()
	UNPCRegistry* Registry;
};
