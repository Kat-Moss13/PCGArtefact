// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EActivityState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EActivityState : uint8
{
	Resting,
	Working
};
