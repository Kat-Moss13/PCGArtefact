#include "ANPCSpawner.h"
#include "ANPCCharacter.h"
#include "ANPCPopulationManager.h"
#include "EngineUtils.h"
#include "FNPCMetadataAttributes.h"
#include "FNPCNamePool.h"
#include "FNPCState.h"
#include "INPCPopulationManagerInterface.h"
#include "UWeightedRandomLibrary.h"
#include "Data/PCGBasePointData.h"
#include "Metadata/PCGMetadata.h"


ANPCSpawner::ANPCSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANPCSpawner::SpawnNPCFromPCGData(const UPCGBasePointData* PointData, TArray<FNPCNamePool> DataTableNames)
{
	if (!PointData) return;
	
	const UPCGMetadata* Metadata = PointData->ConstMetadata();
	if (!Metadata) return;

	const FNPCMetadataAttributes Attributes = FNPCMetadataAttributes::Create(Metadata);
	
	const TConstPCGValueRange<FTransform> Transforms = PointData->GetConstTransformValueRange();
	const TConstPCGValueRange<int64> Entries = PointData->GetConstMetadataEntryValueRange();

	for (int i = 0; i < PointData->GetNumPoints(); ++i)
	{
		const FNPCState State = CreateNPCState(i + 1, Entries[i], PointData->GetSeed(i), Attributes, DataTableNames[i]);
		
		SpawnNPCFromPCG(Transforms[i].GetLocation(), Transforms[i].Rotator(), State);
	}
	
	for (TActorIterator<ANPCPopulationManager> It(GetWorld()); It;)
	{
		INPCPopulationManagerInterface::Execute_RegisterNPCs(*It);
		break;
	}
}

TArray<FNPCNamePool> ANPCSpawner::CreateUniqueNames(UDataTable* NameTable, const int32 NumNames, const int32 Seed)
{
	return GetRowsFromNames(NameTable, GetUniqueNamesFromPool(NameTable->GetRowNames(), NumNames, Seed));
}

TArray<FName> ANPCSpawner::GetUniqueNamesFromPool(TArray<FName> AvailableRows, const int32 NumNames, const int32 Seed)
{
	FRandomStream RandomStream(Seed);

	TArray<FName> SelectedRows;

	while (!AvailableRows.IsEmpty() && SelectedRows.Num() < NumNames)
	{
		const int32 Index = RandomStream.RandRange(0, AvailableRows.Num() - 1);

		SelectedRows.Add(AvailableRows[Index]);
		AvailableRows.RemoveAtSwap(Index);
	}

	return SelectedRows;
}

TArray<FNPCNamePool> ANPCSpawner::GetRowsFromNames(const UDataTable* NameTable, const TArray<FName>& RowNames)
{
	TArray<FNPCNamePool> Result;

	for (const FName& RowName : RowNames)
	{
		if (const FNPCNamePool* Row = NameTable->FindRow<FNPCNamePool>(RowName, TEXT("GetRowsFromNames")))
		{
			Result.Add(*Row);
		}
	}

	return Result;
}

void ANPCSpawner::SetWeightedRanges(TArray<float> HealthRanges, TArray<float> AgeRanges, TArray<float> MoodRanges)
{
	StartingHealthRange =
		{
		FWeightedRange{0.0f,0.1f, HealthRanges[0]},
		FWeightedRange{0.1f,0.3f, HealthRanges[1]},
		FWeightedRange{0.3f,0.5f, HealthRanges[2]},
		FWeightedRange{0.5f,1.0f, HealthRanges[3]}
		};
	StartingMoodRange =
		{
		FWeightedRange{0.0f,0.1f, MoodRanges[0]},
		FWeightedRange{0.1f,0.4f, MoodRanges[1]},
		FWeightedRange{0.4f,0.6f, MoodRanges[2]},
		FWeightedRange{0.6f,0.9f, MoodRanges[3]},
		FWeightedRange{0.9f,1.0f, MoodRanges[4]},
	};
	StartingAgeRange =
		{
		FWeightedRange{1.0f,8.0f, AgeRanges[0]},
		FWeightedRange{8.0f,15.0f, AgeRanges[1]},
		FWeightedRange{15.0f,25.0f, AgeRanges[2]},
		FWeightedRange{25.0f,60.0f, AgeRanges[3]},
		FWeightedRange{60.0f,80.0f, AgeRanges[4]},
	};
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

FNPCState ANPCSpawner::CreateNPCState(const int32 NPCID, const int64 MetadataKey, const int32 Seed, const FNPCMetadataAttributes& Attributes, FNPCNamePool DataTableName) const
{
	FNPCState State;

	State.ID = NPCID;
	State.FirstName = DataTableName.FirstName;
	State.Surname = DataTableName.Surname;
	State.Sex = DataTableName.Sex;
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


