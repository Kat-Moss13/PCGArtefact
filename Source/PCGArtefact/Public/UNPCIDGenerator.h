// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UNPCIDGenerator.generated.h"

/**
 * 
 */
UCLASS()
class PCGARTEFACT_API UNPCIDGenerator : public UObject
{
	GENERATED_BODY()

public:
	int GenerateID() const;
	
private:
	int NextID = 0;
};
