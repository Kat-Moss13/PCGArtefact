#include "ANPCPopulationManager.h"

#include "ANPCGameMode.h"
#include "EngineUtils.h"
#include "UTimeManagementComponent.h"


ANPCPopulationManager::ANPCPopulationManager()
{
	PrimaryActorTick.bCanEverTick = false;

	StateEvaluator = CreateDefaultSubobject<UNPCStateEvaluatorComponent>(TEXT("StateEvaluator"));
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
	UE_LOG(LogTemp, Warning, TEXT("Registry Size: %d"), NPCRegistry.Num());
	TArray<ANPCCharacter*> NPCs;
    
    for (const TPair<int32, TObjectPtr<ANPCCharacter>>& Pair : NPCRegistry)
    {
        NPCs.Add(Pair.Value.Get());
    }
    
    return NPCs;
}

void ANPCPopulationManager::HandleHourChanged(const int32 CurrentHour)
{
	if(!StateEvaluator) return;
	
	StateEvaluator->EvaluateAllNPCs(GetNPCs_Implementation(), CurrentHour);
}


void ANPCPopulationManager::BeginPlay()
{
	Super::BeginPlay();
	ANPCGameMode* GameMode = Cast<ANPCGameMode>(GetWorld()->GetAuthGameMode());

	if (!GameMode || !GameMode->TimeManagement)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Time Management Component"));
		return;
	}

	GameMode->TimeManagement->OnHourChanged.AddDynamic(this, &ANPCPopulationManager::HandleHourChanged);

	UE_LOG(LogTemp, Warning, TEXT("Bound to Hour Changed"));
}


