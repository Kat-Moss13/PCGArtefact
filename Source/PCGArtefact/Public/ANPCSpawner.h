// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FNPCNamePool.h"
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
	void SpawnNPCFromPCGData(const UPCGBasePointData* PointData, TArray<FNPCNamePool> DataTableNames) const;

	UFUNCTION(BlueprintCallable, Category = "Names")
	static TArray<FNPCNamePool> CreateUniqueNames(UDataTable* NameTable, int32 NumNames, const int32 Seed);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranges")
	TArray<FWeightedRange> StartingAgeRange =
		{
			FWeightedRange{1.0f,8.0f, 5.0f},
			FWeightedRange{8.0f,15.0f, 15.0f},
			FWeightedRange{15.0f,25.0f, 35.0f},
			FWeightedRange{25.0f,60.0f, 30.0f},
			FWeightedRange{60.0f,80.0f, 15.0f},
		};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranges")
	TArray<FWeightedRange> StartingMoodRange =
		{
			FWeightedRange{0.0f,0.1f, 5.0f},
			FWeightedRange{0.1f,0.4f, 15.0f},
			FWeightedRange{0.4f,0.6f, 35.0f},
			FWeightedRange{0.6f,0.9f, 30.0f},
			FWeightedRange{0.9f,1.0f, 15.0f},
		};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranges")
	TArray<FWeightedRange> StartingHealthRange =
		{
			FWeightedRange{0.0f,0.1f, 3.0f},
			FWeightedRange{0.1f,0.3f, 7.0f},
			FWeightedRange{0.3f,0.5f, 10.0f},
			FWeightedRange{0.5f,1.0f, 70.0f}
		};
	
	static TArray<FName> GetUniqueNamesFromPool(TArray<FName> AvailableRows, int32 NumNames, const int32 Seed);
	
private:
	void SpawnNPCFromPCG(const FVector& Location, const FRotator& Rotation, const FNPCState& InitialState) const;
	FNPCState CreateNPCState(int32 NPCID,int64 MetadataKey,int32 Seed,const FNPCMetadataAttributes& Attributes, FNPCNamePool DataTableName) const;
	void InitialiseNPCStateValues(FNPCState& State, int32 Seed) const;
	
	static TArray<FNPCNamePool> GetRowsFromNames(const UDataTable* NameTable, const TArray<FName>& RowNames);
	
	UPROPERTY(EditAnywhere, Category="NPC")
	TSubclassOf<ANPCCharacter> NPCClass;

	UFUNCTION(BlueprintCallable)
	void SetWeightedRanges(TArray<float> HealthRanges, TArray<float> AgeRanges, TArray<float> MoodRanges);
};
