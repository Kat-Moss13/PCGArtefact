// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

struct FNPCMoodTags : public FGameplayTagNativeAdder
{
	FGameplayTag Overjoyed;
	FGameplayTag Happy;
	FGameplayTag Fine;
	FGameplayTag Sad;
	FGameplayTag Depressed;

	FORCEINLINE  static const FNPCMoodTags& Get() { return NPCMoodTags; }

protected:
	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

		Overjoyed = Manager.AddNativeGameplayTag("Overjoyed");
		Happy = Manager.AddNativeGameplayTag("Happy");
		Fine = Manager.AddNativeGameplayTag("Fine");
		Sad = Manager.AddNativeGameplayTag("Sad");
		Depressed = Manager.AddNativeGameplayTag("Depressed");
	}
private:
	static FNPCMoodTags NPCMoodTags;
 };
