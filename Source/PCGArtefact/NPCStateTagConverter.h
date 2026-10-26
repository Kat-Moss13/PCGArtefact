// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EAgeState.h"
#include "EHealthState.h"
#include "EJobType.h"
#include "EMoodState.h"
#include "EPersonalityType.h"
#include "ERelationshipState.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
class NPCStateTagConverter
{
public:
	static FGameplayTag GetAgeTag(EAgeState AgeState);
	static FGameplayTag GetHealthTag(EHealthState HealthState);
	static FGameplayTag GetJobTag(EJobType JobType);
	static FGameplayTag GetMoodTag(EMoodState MoodState);
	static FGameplayTag GetPersonalityTag(EPersonalityType PersonalityType);
	static FGameplayTag GetRelationshipTag(ERelationshipState RelationshipState);
};
