// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ANPCCharacter.h"
#include "FNPCActivity.h"
#include "FNPCJobPool.h"
#include "FPersonalityTemplate.h"
#include "Components/ActorComponent.h"
#include "UNPCStateEvaluatorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PCGARTEFACT_API UNPCStateEvaluatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNPCStateEvaluatorComponent();

	
	void EvaluateAllNPCs(TArray<ANPCCharacter*> NPCs, const int32 CurrentHour);
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	TObjectPtr<UDataTable> PersonalityTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	TObjectPtr<UDataTable> JobTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	TObjectPtr<UDataTable> ActivityTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evaluation")
	int32 AgeUpdateIntervalHours = 6;
	
private:
	void EvaluateNPC(ANPCCharacter* npc, int32 CurrentHour);
	bool IsWorkingHour(FNPCJobPool& JobData, int32 CurrentHour) const;

	float CalculateMoodChange(const FNPCState& State, FPersonalityTemplate& Personality, FNPCJobPool& Job, FNPCActivity& Activity) const;
	float CalculateHealthChange(const FNPCState& State, FNPCJobPool& Job, FNPCActivity& Activity) const;

};
