// Fill out your copyright notice in the Description page of Project Settings.


#include "ANPCSpawner.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "ANPCCharacter.h"
#include "InterchangeResult.h"
#include "UNPCIDGenerator.h"
#include "UNPCRegistry.h"
#include "Data/PCGBasePointData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Metadata/PCGMetadata.h"
#include "opensubdiv/far/error.h"


ANPCSpawner::ANPCSpawner(): Registry(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANPCSpawner::SpawnNPCFromPCGData(const UPCGBasePointData* pointData)
{
	if (!pointData) return;
	
	const UPCGMetadata* Metadata = pointData->ConstMetadata();
	if (!Metadata) return;

	const FPCGMetadataAttribute<FString>* FirstNameAttribute = Metadata->GetConstTypedAttribute<FString>("FirstName");
	const FPCGMetadataAttribute<FString>* SurnameAttribute = Metadata->GetConstTypedAttribute<FString>("Surname");
	const FPCGMetadataAttribute<bool>* SexAttribute = Metadata->GetConstTypedAttribute<bool>("Sex");
	const FPCGMetadataAttribute<int64>* PersonalityAttribute = Metadata->GetConstTypedAttribute<int64>("PersonalityType");
	const FPCGMetadataAttribute<int64>* JobAttribute = Metadata->GetConstTypedAttribute<int64>("JobType");
	
	const TConstPCGValueRange<FTransform> Transforms = pointData->GetConstTransformValueRange();
	const TConstPCGValueRange<int64> Entries = pointData->GetConstMetadataEntryValueRange();
	
	for (int i = 1; i < pointData->GetNumPoints(); ++i)
	{
		const FTransform& Transform = Transforms[i];
		const int Key = Entries[i];
		
		FNPCState State;
		State.ID = i;
		int32 pointSeed = pointData->GetSeed(i);
		srand(pointSeed);
		
		if (FirstNameAttribute) State.FirstName = FirstNameAttribute->GetValueFromItemKey(Key);
		if (SurnameAttribute) State.Surname = SurnameAttribute->GetValueFromItemKey(Key);
		if (SexAttribute) State.Sex = SexAttribute->GetValueFromItemKey(Key);
		if (PersonalityAttribute) State.PersonalityType = static_cast<EPersonalityType>(PersonalityAttribute->GetValueFromItemKey(Key));
		if (JobAttribute) State.JobType = static_cast<EJobType>(JobAttribute->GetValueFromItemKey(Key));
		
		
		
		SpawnNPCFromPCG(Transform.GetLocation(), Transform.Rotator(), State);
	}
}

void ANPCSpawner::SpawnNPCFromPCG(const FVector& location, const FRotator& rotation,
                                            const FNPCState& initialState) const
{
	if(!NPCClass) return;
	
	// Registry = GetWorld()->GetGameInstance()->GetSubsystem<UNPCRegistry>();
	// if (!Registry) return nullptr;
	
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	ANPCCharacter* NPC = GetWorld()->SpawnActor<ANPCCharacter>(NPCClass, location, rotation, spawnParameters);
	if (!NPC) return;
	
	//int NewID = Registry->IDGenerator->GenerateID();
	
	FNPCState StateCopy = initialState;
	StateCopy.UpdateAllEnums();
	
	NPC->InitialiseFromState(StateCopy);
	
	// if (Registry)
	// {
	// 	Registry->RegisterNPC(NewID, NPC, StateCopy);
	// }

	//return NPC;
}


