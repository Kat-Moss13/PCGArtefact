// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

struct FNPCRelationshipTags : public FGameplayTagNativeAdder
{
	FGameplayTag Friends;
	FGameplayTag Love;
	FGameplayTag Hate;
	FGameplayTag Neutral;
	FGameplayTag Dislike;
	FGameplayTag Married;

	FORCEINLINE  static const FNPCRelationshipTags& Get() { return NPCRelationshipTags; }

protected:
	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

		Friends = Manager.AddNativeGameplayTag("Friends");
		Hate = Manager.AddNativeGameplayTag("Hate");
		Love = Manager.AddNativeGameplayTag("Love");
		Neutral = Manager.AddNativeGameplayTag("Neutral");
		Married = Manager.AddNativeGameplayTag("Married");
		Dislike = Manager.AddNativeGameplayTag("Dislike");
	}
private:
	static FNPCRelationshipTags NPCRelationshipTags;
 };
