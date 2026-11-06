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

	
	void EvaluateAllNPCs(TArray<ANPCCharacter*> NPCs, const int32 CurrentHour) const;
	
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
	void EvaluateNPC(ANPCCharacter* NPC, int32 CurrentHour) const;
	static bool IsWorkingHour(const FNPCJobPool& JobData, int32 CurrentHour);

	static float CalculateMoodChange(const FNPCState& State, const FPersonalityTemplate& Personality, const FNPCJobPool& Job, const FNPCActivity& Activity);
	static float CalculateHealthChange(const FNPCState& State, const FNPCJobPool& Job, const FNPCActivity& Activity);

};
