#include "ANPCPopulationManager.h"
#include "ANPCGameMode.h"
#include "EngineUtils.h"
#include "UTimeManagementComponent.h"
#include "Kismet/GameplayStatics.h"


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
		NPC->RerunConstructionScripts();
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

void ANPCPopulationManager::HandleHourChanged(const int32 CurrentHour)
{
	if(!StateEvaluator) return;
	
	StateEvaluator->EvaluateAllNPCs(GetNPCs_Implementation(), CurrentHour);
	
}

void ANPCPopulationManager::HandleMoodChanged_Implementation(ANPCCharacter* NPC, EMoodState Mood)
{
}

void ANPCPopulationManager::HandleHealthChanged_Implementation(ANPCCharacter* NPC, EHealthState Health)
{
}

void ANPCPopulationManager::HandleHealthUpdated(EHealthState Health, int32 NPC_ID)
{
	HandleHealthChanged(GetNPCByID_Implementation(NPC_ID), Health);
}

void ANPCPopulationManager::BeginPlay()
{
	Super::BeginPlay();
	ANPCGameMode* GameMode = Cast<ANPCGameMode>(GetWorld()->GetAuthGameMode());

	if (!GameMode || !GameMode->TimeManagement)
	{
		return;
	}

	GameMode->TimeManagement->OnHourChanged.AddDynamic(this, &ANPCPopulationManager::HandleHourChanged);
	StateEvaluator->OnMoodChanged.AddDynamic(this, &ANPCPopulationManager::HandleMoodUpdated);
	StateEvaluator->OnHealthChanged.AddDynamic(this, &ANPCPopulationManager::HandleHealthUpdated);
	
	TArray<ANPCCharacter*> NPCs = GetNPCs_Implementation();
	for (ANPCCharacter* NPC : NPCs)
	{
		FNPCState State = NPC->GetNPCState_Implementation();
		HandleMoodChanged(NPC, State.MoodState);
		HandleHealthChanged(NPC, State.HealthState);
	}
}

void ANPCPopulationManager::HandleMoodUpdated(EMoodState Mood, int32 NPC_ID)
{
	HandleMoodChanged(GetNPCByID_Implementation(NPC_ID), Mood);
}



