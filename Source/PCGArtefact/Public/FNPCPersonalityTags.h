// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

struct FNPCPersonalityTags : public FGameplayTagNativeAdder
{
	FGameplayTag Friendly;
	FGameplayTag Flirty;
	FGameplayTag Gloomy;
	FGameplayTag Introverted;
	FGameplayTag Extroverted;
	FGameplayTag Relaxed;
	FGameplayTag Crabby;

	FORCEINLINE  static const FNPCPersonalityTags& Get() { return NPCPersonalityTags; }

protected:
	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

		Friendly = Manager.AddNativeGameplayTag("Friendly");
		Flirty = Manager.AddNativeGameplayTag("Flirty");
		Gloomy = Manager.AddNativeGameplayTag("Gloomy");
		Introverted = Manager.AddNativeGameplayTag("Introverted");
		Extroverted = Manager.AddNativeGameplayTag("Extroverted");
		Relaxed = Manager.AddNativeGameplayTag("Relaxed");
		Crabby = Manager.AddNativeGameplayTag("Crabby");
	}
private:
	static FNPCPersonalityTags NPCPersonalityTags;
 };
