#include "ANPCPopulationManager.h"

#include "EngineUtils.h"


ANPCPopulationManager::ANPCPopulationManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANPCPopulationManager::RegisterNPCs_Implementation()
{
	NPCRegistry.Empty();
	RegisteredIDs.Empty();

	const UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}
	
	for (TActorIterator<ANPCCharacter> It(World); It; ++It)
    {
        ANPCCharacter* NPC = *It;

        if (!NPC->Implements<UNPCInterface>())
        {
            continue;
        }

        const int32 NPC_ID = INPCInterface::Execute_GetNPCID(NPC);

        NPCRegistry.Add(NPC_ID, NPC);
        RegisteredIDs.Add(NPC_ID);
    }
    
    RegisteredNPCCount = NPCRegistry.Num();
}

int32 ANPCPopulationManager::GetNPCCount_Implementation()
{
	return RegisteredNPCCount;
}

ANPCCharacter* ANPCPopulationManager::GetNPCByID_Implementation(int32 NPC_ID)
{
	if (const TObjectPtr<ANPCCharacter>* Found = NPCRegistry.Find(NPC_ID))
	{
		return Found->Get();
	}

	return nullptr;
}

TArray<ANPCCharacter*> ANPCPopulationManager::GetNPCs_Implementation()
{
	TArray<ANPCCharacter*> NPCs;
    
    for (const TPair<int32, TObjectPtr<ANPCCharacter>>& Pair : NPCRegistry)
    {
        NPCs.Add(Pair.Value.Get());
    }
    
    return NPCs;
}

// Called when the game starts or when spawned
void ANPCPopulationManager::BeginPlay()
{
	Super::BeginPlay();
	
}


