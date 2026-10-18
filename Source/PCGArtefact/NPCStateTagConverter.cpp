// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCStateTagConverter.h"
#include "FNPCAgeTags.h"
#include "FNPCHealthTags.h"
#include "FNPCJobTags.h"
#include "FNPCMoodTags.h"
#include "FNPCPersonalityTags.h"
#include "FNPCRelationshipTags.h"

FGameplayTag NPCStateTagConverter::GetAgeTag(EAgeState ageState)
{
	const FNPCAgeTags& Tags = FNPCAgeTags::Get();
	
	switch(ageState)
	{
		case EAgeState::Child: return Tags.Child;
		case EAgeState::Baby: return Tags.Baby;
		case EAgeState::YoungAdult: return Tags.YoungAdult;
		case EAgeState::Elder: return Tags.Elder;
		case EAgeState::Adult:
		default: return Tags.Adult;
	}
}

FGameplayTag NPCStateTagConverter::GetHealthTag(EHealthState healthState)
{
	const FNPCHealthTags& Tags = FNPCHealthTags::Get();
	switch(healthState)
	{
		case EHealthState::Critical: return Tags.Critical;
		case EHealthState::Injured: return Tags.Injured;
		case EHealthState::Sick: return Tags.Sick;
		case EHealthState::Healthy:
		default: return Tags.Healthy;
	}
}

FGameplayTag NPCStateTagConverter::GetJobTag(EJobType jobType)
{
	const FNPCJobTags& Tags = FNPCJobTags::Get();
	switch(jobType)
	{
		case EJobType::Baker: return Tags.Baker;
		case EJobType::Farmer : return Tags.Farmer;
		case EJobType::ShopKeeper: return Tags.ShopKeeper;
		case EJobType::Librarian: return Tags.Librarian;
		case EJobType::Toolsmith: return Tags.Toolsmith;
		case EJobType::Unemployed:
		default: return Tags.Unemployed;
	}
}

FGameplayTag NPCStateTagConverter::GetMoodTag(EMoodState moodState)
{
	const FNPCMoodTags& Tags = FNPCMoodTags::Get();
	switch(moodState)
	{
		case EMoodState::Depressed: return Tags.Depressed;
		case EMoodState::Fine: return Tags.Fine;
		case EMoodState::Overjoyed: return Tags.Overjoyed;
		case EMoodState::Sad: return Tags.Sad;
		case EMoodState::Happy:
		default: return Tags.Happy;
	}
}

FGameplayTag NPCStateTagConverter::GetPersonalityTag(EPersonalityType personalityType)
{
	const FNPCPersonalityTags& Tags = FNPCPersonalityTags::Get();
	switch(personalityType)
	{
		case EPersonalityType::Crabby: return Tags.Crabby;
		case EPersonalityType::Extroverted: return Tags.Extroverted;
		case EPersonalityType::Flirty: return Tags.Flirty;
		case EPersonalityType::Gloomy: return Tags.Gloomy;
		case EPersonalityType::Introverted: return Tags.Introverted;
		case EPersonalityType::Relaxed: return Tags.Relaxed;
		case EPersonalityType::Friendly:
		default: return Tags.Friendly;
	}
}

FGameplayTag NPCStateTagConverter::GetRelationshipTag(ERelationshipState relationshipState)
{
	const FNPCRelationshipTags& Tags = FNPCRelationshipTags::Get();
	switch(relationshipState)
	{
		case ERelationshipState::Hate: return Tags.Hate;
		case ERelationshipState::Love: return Tags.Love;
		case ERelationshipState::Married: return Tags.Married;
		case ERelationshipState::Friends: return Tags.Friends;
		case ERelationshipState::Dislike: return Tags.Dislike;
		case ERelationshipState::Neutral:
		default: return Tags.Neutral;
	}
}
