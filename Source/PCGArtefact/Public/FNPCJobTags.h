// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

struct FNPCJobTags : public FGameplayTagNativeAdder
{
	FGameplayTag Farmer;
	FGameplayTag ShopKeeper;
	FGameplayTag Unemployed;
	FGameplayTag Baker;
	FGameplayTag Librarian;
	FGameplayTag Toolsmith;

	FORCEINLINE  static const FNPCJobTags& Get() { return NPCJobTags; }

protected:
	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

		Farmer = Manager.AddNativeGameplayTag("Farmer");
		ShopKeeper = Manager.AddNativeGameplayTag("Shop Keeper");
		Unemployed = Manager.AddNativeGameplayTag("Unemployed");
		Baker = Manager.AddNativeGameplayTag("Baker");
		Librarian = Manager.AddNativeGameplayTag("Librarian");
		Toolsmith = Manager.AddNativeGameplayTag("Toolsmith");
	}
private:
	static FNPCJobTags NPCJobTags;
 };
