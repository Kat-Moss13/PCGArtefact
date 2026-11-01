// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EJobType.h"
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
	int32 WorkStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WorkEndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> WorkplaceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoodModifer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthModifer;
};
