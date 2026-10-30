// Fill out your copyright notice in the Description page of Project Settings.


#include "ANPCSpawner.h"

#include "ANPCCharacter.h"
#include "ANPCPopulationManager.h"
#include "EngineUtils.h"
#include "FNPCMetadataAttributes.h"
#include "FNPCState.h"
#include "INPCPopulationManagerInterface.h"
#include "InterchangeResult.h"
#include "UWeightedRandomLibrary.h"
#include "Data/PCGBasePointData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Metadata/PCGMetadata.h"


ANPCSpawner::ANPCSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANPCSpawner::SpawnNPCFromPCGData(const UPCGBasePointData* PointData) const
{
	if (!PointData) return;
	
	const UPCGMetadata* Metadata = PointData->ConstMetadata();
	if (!Metadata) return;
	
	const FNPCMetadataAttributes Attributes = FNPCMetadataAttributes::Create(Metadata);
	
	const TConstPCGValueRange<FTransform> Transforms = PointData->GetConstTransformValueRange();
	const TConstPCGValueRange<int64> Entries = PointData->GetConstMetadataEntryValueRange();

	for (int i = 1; i <= PointData->GetNumPoints(); ++i)
	{
		const FNPCState State = CreateNPCState(i, Entries[i], PointData->GetSeed(i), Attributes);
		
		SpawnNPCFromPCG(Transforms[i].GetLocation(), Transforms[i].Rotator(), State);
	}
	
	for (TActorIterator<ANPCPopulationManager> It(GetWorld()); It; ++It)
	{
		INPCPopulationManagerInterface::Execute_RegisterNPCs(*It);
		break;
	}
}

void ANPCSpawner::SpawnNPCFromPCG(const FVector& Location, const FRotator& Rotation, const FNPCState& InitialState) const
{
	if(!NPCClass) return;
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	ANPCCharacter* NPC = GetWorld()->SpawnActor<ANPCCharacter>(NPCClass, Location, Rotation, SpawnParameters);
	if (!NPC) return;
	
	FNPCState StateCopy = InitialState;
	StateCopy.UpdateAllEnums();
	
	NPC->InitialiseFromState(StateCopy);
}

FNPCState ANPCSpawner::CreateNPCState(const int32 NPCID, const int64 MetadataKey, const int32 Seed, const FNPCMetadataAttributes& Attributes) const
{
	FNPCState State;

	State.ID = NPCID;
	State.FirstName = Attributes.FirstName->GetValueFromItemKey(MetadataKey);
	State.Surname = Attributes.Surname->GetValueFromItemKey(MetadataKey);
	State.Sex = Attributes.Sex->GetValueFromItemKey(MetadataKey);
	State.PersonalityType = static_cast<EPersonalityType>(Attributes.Personality->GetValueFromItemKey(MetadataKey));
	State.JobType = static_cast<EJobType>(Attributes.Job->GetValueFromItemKey(MetadataKey));

	InitialiseNPCStateValues(State, Seed);

	return State;
}

void ANPCSpawner::InitialiseNPCStateValues(FNPCState& State, const int32 Seed) const
{
	FRandomStream RandomStream(Seed);

	State.SetAgeValue(UWeightedRandomLibrary::GetWeightedRandomFloat(StartingAgeRange, RandomStream));
	State.SetHealthValue(UWeightedRandomLibrary::GetWeightedRandomFloat(StartingHealthRange, RandomStream));
	State.SetMoodValue(UWeightedRandomLibrary::GetWeightedRandomFloat(StartingMoodRange, RandomStream));

	State.JobSatisfaction = 1.0f;
	State.LastInteractionTime = 0.0f;
	State.LastNPCInteractionID = 0;
	
	if (State.AgeState == EAgeState::Baby || State.AgeState == EAgeState::Child)
	{
		State.JobType = EJobType::Unemployed;
	}
}


