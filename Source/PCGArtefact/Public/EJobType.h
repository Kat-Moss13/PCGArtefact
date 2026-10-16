// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EJobType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EJobType : uint8
{
	Farmer,
	ShopKeeper,
	Unemployed,
	Baker,
	Librarian,
	Toolsmith
};
