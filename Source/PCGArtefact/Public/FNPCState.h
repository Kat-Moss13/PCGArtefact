#pragma once

#include "CoreMinimal.h"
#include "EAgeState.h"
#include "EHealthState.h"
#include "EJobType.h"
#include "EMoodState.h"
#include "EPersonalityType.h"
#include "ERelationshipState.h"
#include "GameplayTagContainer.h"
#include "ShaderCompilerJobTypes.h"
#include "PCGArtefact/NPCStateTagConverter.h"
#include "FNPCState.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FNPCState
{
	GENERATED_BODY()

	UPROPERTY()
	int AgeYears = 0;
	
	UPROPERTY()
	float HealthValue = 0.0f;

	UPROPERTY()
	float MoodValue = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAgeState AgeState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EHealthState HealthState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JobSatisfaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EJobType JobType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LastNPCInteractionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LastInteractionTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMoodState MoodState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, int> NPC_IDRelationshipScore;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FirstName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Surname;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPersonalityType PersonalityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Sex; //yes for female, no for male
	
	void SetAgeValue(int newAgeValue);
	void SetHealthValue(float newHealthValue);
	void SetMoodValue(float newMoodValue);
	ERelationshipState GetRelationshipEnum(int ID) const;
	int GetRelationshipScore(int ID) const;
	void ModifyRelationshipScore(int ID, int score);
	void GetAllTags(FGameplayTagContainer& outTags) const;
	void UpdateAllEnums();
	
	FString ToString() const
	{
		return FString::Printf(
			TEXT("Name: %s | Age: %d | Mood: %d | Health: %d | Job: %d | Personality: %d"),
			*FirstName, AgeYears,
			static_cast<int>(MoodState), static_cast<int>(HealthState),
			static_cast<int>(JobType), static_cast<int>(PersonalityType)
		);
	}

private:
	void UpdateAgeState();
	void UpdateHealthState();
	void UpdateMoodState();
};

inline void FNPCState::SetAgeValue(int newAgeValue)
{
	AgeYears = newAgeValue;
	UpdateAgeState();
}

inline void FNPCState::SetHealthValue(float newHealthValue)
{
	HealthValue = newHealthValue;
	UpdateHealthState();
}

inline void FNPCState::SetMoodValue(float newMoodValue)
{
	MoodValue = newMoodValue;
	UpdateMoodState();
}

inline ERelationshipState FNPCState::GetRelationshipEnum(int NPC_ID) const
{
	const int Score = GetRelationshipScore(NPC_ID);

	if (Score >= 0.9) return ERelationshipState::Married;
	if (Score >= 0.75) return ERelationshipState::Love;
	if (Score >= 0.4) return ERelationshipState::Friends;
	if (Score >= 0) return ERelationshipState::Neutral;
	if (Score >= -0.3) return ERelationshipState::Dislike;
	if (Score < -0.3) return ERelationshipState::Hate;
	return ERelationshipState::Hate;
}

inline int FNPCState::GetRelationshipScore(int NPC_ID) const
{
	return NPC_IDRelationshipScore.FindRef(NPC_ID);
}

inline void FNPCState::ModifyRelationshipScore(int NPC_ID, int score)
{
	NPC_IDRelationshipScore.FindOrAdd(NPC_ID) += score;
}

inline void FNPCState::GetAllTags(FGameplayTagContainer& outTags) const
{
	outTags.AddTag(NPCStateTagConverter::GetAgeTag(AgeState));
	outTags.AddTag(NPCStateTagConverter::GetHealthTag(HealthState));
	outTags.AddTag(NPCStateTagConverter::GetJobTag(JobType));
	outTags.AddTag(NPCStateTagConverter::GetMoodTag(MoodState));
	outTags.AddTag(NPCStateTagConverter::GetPersonalityTag(PersonalityType));
}

inline void FNPCState::UpdateAgeState()
{
	if(AgeYears <= 2) AgeState = EAgeState::Baby;
	if(AgeYears < 15 && AgeYears >= 3) AgeState = EAgeState::Child;
	if(AgeYears < 26 && AgeYears >= 15) AgeState = EAgeState::YoungAdult;
	if(AgeYears < 60 && AgeYears >= 26) AgeState = EAgeState::Adult;
	if(AgeYears >= 61) AgeState = EAgeState::Elder;
}

inline void FNPCState::UpdateHealthState()
{
	if(HealthValue <= 0.1f) HealthState = EHealthState::Critical;
	if(HealthValue < 0.3f && HealthValue >= 0.2f) HealthState = EHealthState::Sick;
	if(HealthValue < 0.5f && HealthValue >= 0.3f) HealthState = EHealthState::Injured;
	if(HealthValue >= 0.5) HealthState = EHealthState::Healthy;
}

inline void FNPCState::UpdateMoodState()
{
	if(MoodValue <= 0.1f) MoodState = EMoodState::Depressed;
	if(MoodValue < 0.4f && MoodValue >= 0.2f) MoodState = EMoodState::Sad;
	if(MoodValue < 0.6f && MoodValue >= 0.4f) MoodState = EMoodState::Fine;
	if(MoodValue < 0.9f && MoodValue >= 0.6f) MoodState = EMoodState::Happy;
	if(MoodValue >= 0.9f) MoodState = EMoodState::Overjoyed;
}

inline void FNPCState::UpdateAllEnums()
{
	UpdateAgeState();
	UpdateHealthState();
	UpdateMoodState();
}
