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


USTRUCT(BlueprintType)
struct FNPCState
{
	GENERATED_BODY()

	UPROPERTY()
	int32 AgeYears = 0;
	
	UPROPERTY()
	float HealthValue = 0.0f;

	UPROPERTY()
	float MoodValue = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAgeState AgeState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EHealthState HealthState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JobSatisfaction; // 0-1

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EJobType JobType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LastNPCInteractionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LastInteractionTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMoodState MoodState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, int32> NPC_IDRelationshipScore;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString FirstName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Surname;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPersonalityType PersonalityType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Sex; //yes for female, no for male
	
	void SetAgeValue(int32 NewAgeValue);
	void SetHealthValue(float NewHealthValue);
	void SetMoodValue(float NewMoodValue);
	ERelationshipState GetRelationshipEnum(int32 NPC_ID) const;
	int32 GetRelationshipScore(int32 NPC_ID) const;
	void ModifyRelationshipScore(int32 NPC_ID, int32 ScoreDelta);
	void GetAllTags(FGameplayTagContainer& OutTags) const;
	void UpdateAllEnums();
	
	FString ToString() const
	{
		return FString::Printf(
			TEXT("Name: %s | Age: %d | Mood: %d | Health: %d | Job: %d | Personality: %d"),
			*FirstName, AgeYears,
			static_cast<int32>(MoodState), static_cast<int32>(HealthState),
			static_cast<int32>(JobType), static_cast<int32>(PersonalityType)
		);
	}

private:
	void UpdateAgeState();
	void UpdateHealthState();
	void UpdateMoodState();
};

inline void FNPCState::SetAgeValue(const int32 NewAgeValue)
{
	AgeYears = NewAgeValue;
	UpdateAgeState();
}

inline void FNPCState::SetHealthValue(const float NewHealthValue)
{
	HealthValue = NewHealthValue;
	UpdateHealthState();
}

inline void FNPCState::SetMoodValue(const float NewMoodValue)
{
	MoodValue = NewMoodValue;
	UpdateMoodState();
}

inline ERelationshipState FNPCState::GetRelationshipEnum(const int32 NPC_ID) const
{
	const int32 Score = GetRelationshipScore(NPC_ID);

	if (Score >= 0.9) return ERelationshipState::Married;
	if (Score >= 0.75) return ERelationshipState::Love;
	if (Score >= 0.4) return ERelationshipState::Friends;
	if (Score >= 0) return ERelationshipState::Neutral;
	if (Score >= -0.3) return ERelationshipState::Dislike;
	if (Score < -0.3) return ERelationshipState::Hate;
	return ERelationshipState::Hate;
}

inline int32 FNPCState::GetRelationshipScore(const int32 NPC_ID) const
{
	return NPC_IDRelationshipScore.FindRef(NPC_ID);
}

inline void FNPCState::ModifyRelationshipScore(const int32 NPC_ID, const int32 ScoreDelta)
{
	NPC_IDRelationshipScore.FindOrAdd(NPC_ID) += ScoreDelta;
}

inline void FNPCState::GetAllTags(FGameplayTagContainer& OutTags) const
{
	OutTags.AddTag(NPCStateTagConverter::GetAgeTag(AgeState));
	OutTags.AddTag(NPCStateTagConverter::GetHealthTag(HealthState));
	OutTags.AddTag(NPCStateTagConverter::GetJobTag(JobType));
	OutTags.AddTag(NPCStateTagConverter::GetMoodTag(MoodState));
	OutTags.AddTag(NPCStateTagConverter::GetPersonalityTag(PersonalityType));
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
