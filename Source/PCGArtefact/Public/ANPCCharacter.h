// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FNPCState.h"
#include "INPCInterface.h"
#include "GameFramework/Character.h"
#include "ANPCCharacter.generated.h"

class UNPCRegistry;

UCLASS()
class PCGARTEFACT_API ANPCCharacter : public ACharacter, public INPCInterface
{
	GENERATED_BODY()

public:
	ANPCCharacter();

	void InitialiseFromState(const FNPCState& NewState);
	
	virtual int32 GetNPCID_Implementation() const override;
	virtual const FNPCState GetNPCState_Implementation() const override;
	virtual void SetNPCState_Implementation(FNPCState NewState) override;

	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	int32 NPC_ID = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	FNPCState NPCState;
	
};
