// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FNPCJobPool.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FNPCJobPool : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EJobType JobType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Surname;
};
