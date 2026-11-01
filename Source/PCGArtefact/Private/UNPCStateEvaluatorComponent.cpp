#include "UNPCStateEvaluatorComponent.h"

#include "ANPCCharacter.h"
#include "FNPCState.h"
#include "InterchangeResult.h"

UNPCStateEvaluatorComponent::UNPCStateEvaluatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNPCStateEvaluatorComponent::EvaluateAllNPCs(TArray<ANPCCharacter*> NPCs)
{
	for (ANPCCharacter* NPC : NPCs)
	{
		FNPCState State = NPC->GetNPCState();
		FString debug = State.ToString();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *debug);
	}
	
}

void UNPCStateEvaluatorComponent::BeginPlay()
{
	Super::BeginPlay();

}


