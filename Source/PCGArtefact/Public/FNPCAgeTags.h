// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

struct FNPCAgeTags : public FGameplayTagNativeAdder
{
	FGameplayTag Baby;
	FGameplayTag Child;
	FGameplayTag YoungAdult;
	FGameplayTag Adult;
	FGameplayTag Elder;

	FORCEINLINE  static const FNPCAgeTags& Get() { return NPCAgeTags; }

protected:
	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

		Baby = Manager.AddNativeGameplayTag("Baby");
		Child = Manager.AddNativeGameplayTag("Child");
		YoungAdult = Manager.AddNativeGameplayTag("Young Adult");
		Adult = Manager.AddNativeGameplayTag("Adult");
		Elder = Manager.AddNativeGameplayTag("Elder");
	}
private:
	static FNPCAgeTags NPCAgeTags;
 };
