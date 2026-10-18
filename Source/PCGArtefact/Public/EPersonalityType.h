// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EPersonalityType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPersonalityType : uint8
{
	Friendly,
	Flirty,
	Gloomy,
	Introverted,
	Extroverted,
	Relaxed,
	Crabby
};
