#include "UNPCStateEvaluatorComponent.h"

#include "ANPCCharacter.h"
#include "FNPCActivity.h"
#include "FNPCJobPool.h"
#include "FNPCState.h"
#include "InterchangeResult.h"

UNPCStateEvaluatorComponent::UNPCStateEvaluatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNPCStateEvaluatorComponent::EvaluateAllNPCs(TArray<ANPCCharacter*> NPCs, const int32 CurrentHour)
{
    for (ANPCCharacter* NPC : NPCs)
    {
        if (!NPC) continue;

        EvaluateNPC(NPC, CurrentHour);
    }
	
}

void UNPCStateEvaluatorComponent::EvaluateNPC(ANPCCharacter* NPC, int32 CurrentHour)
{
	if (!NPC->Implements<UNPCInterface>())
	{
		return;
	}

	FNPCState State = INPCInterface::Execute_GetNPCState(NPC);

	const bool IsWorking = IsWorkingHour(State.JobType, CurrentHour);
	const FName ActivityRowName = IsWorking ? TEXT("Working") : TEXT("Resting");
	const FName JobRowName = GetJobRowName(State.JobType);
	const FName PersonalityRowName = GetPersonalityRowName(State.PersonalityType);
	
	FNPCActivity* Activity = ActivityTable->FindRow<FNPCActivity>(ActivityRowName, TEXT("Activity Lookup"));
	FNPCJobPool* Job = JobTable->FindRow<FNPCJobPool>(JobRowName, TEXT("Job Lookup"));
	FPersonalityTemplate* Personality =PersonalityTable->FindRow<FPersonalityTemplate>(PersonalityRowName,TEXT("Personality Lookup"));
	
	const float MoodChange = CalculateMoodChange(State,*Personality,*Job,*Activity);
	State.SetMoodValue(FMath::Clamp(State.MoodValue + MoodChange,0.0f,1.0f));
	
	const float HealthChange = CalculateHealthChange(State,*Job,*Activity);
	State.SetHealthValue(FMath::Clamp(State.HealthValue + HealthChange,0.0f,1.0f));
	
}

bool UNPCStateEvaluatorComponent::IsWorkingHour(FNPCJobPool& JobData,const int32 CurrentHour) const
{
	return CurrentHour >= JobData.WorkStartTime && CurrentHour < JobData.WorkEndTime;
}

float UNPCStateEvaluatorComponent::CalculateMoodChange(const FNPCState& State, FPersonalityTemplate& Personality,
	FNPCJobPool& Job, FNPCActivity& Activity) const
{
	float MoodChange = 0.0f;

	MoodChange += Personality.MoodModifier;
	MoodChange += Job.MoodModifier;
	MoodChange += Activity.MoodModifier;

	return MoodChange;
}

float UNPCStateEvaluatorComponent::CalculateHealthChange(const FNPCState& State, FNPCJobPool& Job,
	FNPCActivity& Activity) const
{
	float HealthChange = 0.0f;

	HealthChange += Job.HealthModifier;
	HealthChange += Activity.HealthModifier;

	if (State.MoodValue < 0.25f)
	{
		HealthChange -= 0.05f;
	}
	else if (State.MoodValue > 0.75f)
	{
		HealthChange += 0.05f;
	}

	return HealthChange;
}

FName UNPCStateEvaluatorComponent::GetJobRowName(EJobType JobType) const
{
	return UEnum::GetValueAsName(JobType);
}

FName UNPCStateEvaluatorComponent::GetPersonalityRowName(EPersonalityType PersonalityType) const
{
	return UEnum::GetValueAsName(PersonalityType);
}


