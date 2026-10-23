// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ANPCSpawner.generated.h"

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
	void SpawnNPCFromPCGData(const UPCGBasePointData* pointData);
	
private:
	void SpawnNPCFromPCG(const FVector& location, const FRotator& rotation, const FNPCState& initialState) const;

	UPROPERTY()
	UNPCRegistry* Registry;

	UPROPERTY(EditAnywhere, Category="NPC")
	TSubclassOf<ANPCCharacter> NPCClass;
};
