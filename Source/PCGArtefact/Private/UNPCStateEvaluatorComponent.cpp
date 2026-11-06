#include "UNPCStateEvaluatorComponent.h"
#include "ANPCCharacter.h"
#include "FNPCActivity.h"
#include "FNPCJobPool.h"
#include "FNPCState.h"

UNPCStateEvaluatorComponent::UNPCStateEvaluatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNPCStateEvaluatorComponent::EvaluateAllNPCs(TArray<ANPCCharacter*> NPCs, const int32 CurrentHour) const
{
    for (ANPCCharacter* NPC : NPCs)
    {
        if (!NPC) continue;

        EvaluateNPC(NPC, CurrentHour);
    }
	
}

void UNPCStateEvaluatorComponent::EvaluateNPC(ANPCCharacter* NPC, const int32 CurrentHour) const
{
	if (!NPC->Implements<UNPCInterface>())
	{
		return;
	}

	FNPCState State = INPCInterface::Execute_GetNPCState(NPC);
	
	FNPCJobPool* Job = nullptr;
	for (const FName& RowName : JobTable->GetRowNames())
	{
		FNPCJobPool* Row = JobTable->FindRow<FNPCJobPool>(RowName, TEXT("Job Lookup"));

		if (Row && Row->JobType == State.JobType)
		{
			Job = Row;
			break;
		}
	}
	
	const bool IsWorking = IsWorkingHour(*Job, CurrentHour);
	const EActivityState TargetActivity = IsWorking ? EActivityState::Working : EActivityState::Resting;
	
	FPersonalityTemplate* Personality = nullptr;
	for (const FName& RowName : PersonalityTable->GetRowNames())
	{
		FPersonalityTemplate* Row = PersonalityTable->FindRow<FPersonalityTemplate>(RowName, TEXT("Personality Lookup"));

		if (Row && Row->PersonalityType == State.PersonalityType)
		{
			Personality = Row;
			break;
		}
	}

	FNPCActivity* Activity = nullptr;

	for (const FName& RowName : ActivityTable->GetRowNames())
	{
		FNPCActivity* Row = ActivityTable->FindRow<FNPCActivity>(RowName, TEXT("Activity Lookup"));

		if (Row && Row->Activity == TargetActivity)
		{
			Activity = Row;
			break;
		}
	}
	
	const float MoodChange = CalculateMoodChange(State,*Personality,*Job,*Activity);
	State.SetMoodValue(FMath::Clamp(State.MoodValue + MoodChange,0.0f,1.0f));
	
	const float HealthChange = CalculateHealthChange(State,*Job,*Activity);
	State.SetHealthValue(FMath::Clamp(State.HealthValue + HealthChange,0.0f,1.0f));

	UE_LOG(LogTemp, Warning, TEXT("%s | Mood=%.3f | Health=%.3f"), *State.FirstName, State.MoodValue, State.HealthValue);
	NPC->SetNPCState_Implementation(State);
}

bool UNPCStateEvaluatorComponent::IsWorkingHour(const FNPCJobPool& JobData,const int32 CurrentHour)
{
	return CurrentHour >= JobData.WorkStartTime && CurrentHour < JobData.WorkEndTime;
}

float UNPCStateEvaluatorComponent::CalculateMoodChange(const FNPCState& State, const FPersonalityTemplate& Personality,
	const FNPCJobPool& Job, const FNPCActivity& Activity)
{
	float MoodChange = 0.0f;

	MoodChange += Personality.MoodModifier;
	MoodChange += Job.MoodModifier;
	MoodChange += Activity.MoodModifier;
	UE_LOG(LogTemp, Warning, TEXT("%s | MoodChange=%.3f"), *State.FirstName, MoodChange);
	return MoodChange;
}

float UNPCStateEvaluatorComponent::CalculateHealthChange(const FNPCState& State, const FNPCJobPool& Job,
	const FNPCActivity& Activity)
{
	float HealthChange = 0.0f;

	HealthChange += Job.HealthModifier;
	HealthChange += Activity.HealthModifier;

	if (State.MoodValue < 0.05f)
	{
		HealthChange -= 0.05f;
	}
	else if (State.MoodValue > 0.95f)
	{
		HealthChange += 0.05f;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s | Health=%.3f"), *State.FirstName, HealthChange);
	return HealthChange;
	
}

