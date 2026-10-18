// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

struct FNPCHealthTags : public FGameplayTagNativeAdder
{
	FGameplayTag Healthy;
	FGameplayTag Injured;
	FGameplayTag Sick;
	FGameplayTag Critical;

	FORCEINLINE  static const FNPCHealthTags& Get() { return NPCHealthTags; }

protected:
	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

		Healthy = Manager.AddNativeGameplayTag("Healthy");
		Injured = Manager.AddNativeGameplayTag("Injured");
		Sick = Manager.AddNativeGameplayTag("Sick");
		Critical = Manager.AddNativeGameplayTag("Critical");
	}
private:
	static FNPCHealthTags NPCHealthTags;
 };
