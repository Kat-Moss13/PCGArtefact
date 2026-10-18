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
	static FGameplayTag GetAgeTag(EAgeState ageState);
	static FGameplayTag GetHealthTag(EHealthState healthState);
	static FGameplayTag GetJobTag(EJobType jobType);
	static FGameplayTag GetMoodTag(EMoodState moodState);
	static FGameplayTag GetPersonalityTag(EPersonalityType personalityType);
	static FGameplayTag GetRelationshipTag(ERelationshipState relationshipState);
};
