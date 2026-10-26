// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UWeightedRandomLibrary.h"
#include "GameFramework/Actor.h"
#include "Metadata/PCGMetadataAttributeTpl.h"
#include "ANPCSpawner.generated.h"

struct FNPCMetadataAttributes;
class UPCGBasePointData;
class UNPCRegistry;
class UNPCIDGenerator;
struct FNPCState;
class ANPCCharacter;

UCLASS()
class PCGARTEFACT_API ANPCSpawner : public AActor
{
	GENERATED_BODY()

public:
	ANPCSpawner();

	UFUNCTION(BlueprintCallable, Category="Spawner")
	void SpawnNPCFromPCGData(const UPCGBasePointData* PointData) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranges")
	TArray<FWeightedRange> StartingAgeRange =
		{
			FWeightedRange{1,8, 5.0f},
			FWeightedRange{8,15, 15.0f},
			FWeightedRange{15,25, 35.0f},
			FWeightedRange{25,60, 30.0f},
			FWeightedRange{60,80, 15.0f},
		};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranges")
	TArray<FWeightedRange> StartingMoodRange =
		{
			FWeightedRange{0,0.1, 5.0f},
			FWeightedRange{0.1,0.4, 15.0f},
			FWeightedRange{0.4,0.6, 35.0f},
			FWeightedRange{0.6,0.9, 30.0f},
			FWeightedRange{0.9,1, 15.0f},
		};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranges")
	TArray<FWeightedRange> StartingHealthRange =
		{
			FWeightedRange{0,0.1, 3.0f},
			FWeightedRange{0.1,0.3, 7.0f},
			FWeightedRange{0.3,0.5, 10.0f},
			FWeightedRange{0.5,1, 70.0f}
		};
	
private:
	void SpawnNPCFromPCG(const FVector& Location, const FRotator& Rotation, const FNPCState& InitialState) const;
	FNPCState CreateNPCState(int32 NPCID,int64 MetadataKey,int32 Seed,const FNPCMetadataAttributes& Attributes) const;
	void InitialiseNPCStateValues(FNPCState& State, int32 Seed) const;

	UPROPERTY()
	UNPCRegistry* Registry;

	UPROPERTY(EditAnywhere, Category="NPC")
	TSubclassOf<ANPCCharacter> NPCClass;
};
