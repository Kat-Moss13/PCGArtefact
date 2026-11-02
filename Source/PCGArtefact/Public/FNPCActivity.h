// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EActivityState.h"
#include "FNPCActivity.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FNPCActivity : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActivityState Activity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoodModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthModifier;
};
