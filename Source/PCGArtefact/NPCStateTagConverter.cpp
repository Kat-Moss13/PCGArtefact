#include "NPCStateTagConverter.h"

#include "NPCTags.h"

FGameplayTag NPCStateTagConverter::GetAgeTag(const EAgeState AgeState)
{
    switch (AgeState)
    {
        case EAgeState::Baby: return TAG_NPC_Age_Baby;
        case EAgeState::Child: return TAG_NPC_Age_Child;
        case EAgeState::YoungAdult: return TAG_NPC_Age_YoungAdult;
        case EAgeState::Elder: return TAG_NPC_Age_Elder;
        case EAgeState::Adult:
        default: return TAG_NPC_Age_Adult;
    }
}

FGameplayTag NPCStateTagConverter::GetHealthTag(const EHealthState HealthState)
{
    switch (HealthState)
    {
        case EHealthState::Critical: return TAG_NPC_Health_Critical;
        case EHealthState::Injured: return TAG_NPC_Health_Injured;
        case EHealthState::Sick: return TAG_NPC_Health_Sick;
        case EHealthState::Healthy:
        default: return TAG_NPC_Health_Healthy;
    }
}

FGameplayTag NPCStateTagConverter::GetJobTag(const EJobType JobType)
{
    switch (JobType)
    {
        case EJobType::Baker: return TAG_NPC_Job_Baker;
        case EJobType::Farmer: return TAG_NPC_Job_Farmer;
        case EJobType::ShopKeeper: return TAG_NPC_Job_ShopKeeper;
        case EJobType::Librarian: return TAG_NPC_Job_Librarian;
        case EJobType::Toolsmith: return TAG_NPC_Job_Toolsmith;
        case EJobType::Unemployed:
        default: return TAG_NPC_Job_Unemployed;
    }
}

FGameplayTag NPCStateTagConverter::GetMoodTag(const EMoodState MoodState)
{
    switch (MoodState)
    {
        case EMoodState::Overjoyed: return TAG_NPC_Mood_Overjoyed;
        case EMoodState::Fine: return TAG_NPC_Mood_Fine;
        case EMoodState::Depressed: return TAG_NPC_Mood_Depressed;
        case EMoodState::Sad: return TAG_NPC_Mood_Sad;
        case EMoodState::Happy:
        default: return TAG_NPC_Mood_Happy;
    }
}

FGameplayTag NPCStateTagConverter::GetPersonalityTag(const EPersonalityType PersonalityType)
{
    switch (PersonalityType)
    {
        case EPersonalityType::Crabby: return TAG_NPC_Personality_Crabby;
        case EPersonalityType::Extroverted: return TAG_NPC_Personality_Extroverted;
        case EPersonalityType::Flirty: return TAG_NPC_Personality_Flirty;
        case EPersonalityType::Gloomy: return TAG_NPC_Personality_Gloomy;
        case EPersonalityType::Introverted: return TAG_NPC_Personality_Introverted;
        case EPersonalityType::Relaxed: return TAG_NPC_Personality_Relaxed;
        case EPersonalityType::Friendly:
        default: return TAG_NPC_Personality_Friendly;
    }
}

FGameplayTag NPCStateTagConverter::GetRelationshipTag(const ERelationshipState RelationshipState)
{
    switch (RelationshipState)
    {
        case ERelationshipState::Friends: return TAG_NPC_Relationship_Friends;
        case ERelationshipState::Love: return TAG_NPC_Relationship_Love;
        case ERelationshipState::Hate: return TAG_NPC_Relationship_Hate;
        case ERelationshipState::Dislike: return TAG_NPC_Relationship_Dislike;
        case ERelationshipState::Married: return TAG_NPC_Relationship_Married;
        case ERelationshipState::Neutral:
        default: return TAG_NPC_Relationship_Neutral;
    }
}