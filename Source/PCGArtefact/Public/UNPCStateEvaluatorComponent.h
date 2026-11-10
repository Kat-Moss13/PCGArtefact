#pragma once

#include "CoreMinimal.h"
#include "ANPCCharacter.h"
#include "FNPCActivity.h"
#include "FNPCJobPool.h"
#include "FPersonalityTemplate.h"
#include "Components/ActorComponent.h"
#include "EMoodState.h"
#include "UNPCStateEvaluatorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoodChanged, EMoodState, NewMoodState, int32, NPC_ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, EHealthState, NewHealthState, int32, NPC_ID);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PCGARTEFACT_API UNPCStateEvaluatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNPCStateEvaluatorComponent();

	
	void EvaluateAllNPCs(TArray<ANPCCharacter*> NPCs, const int32 CurrentHour);
	
	UPROPERTY(BlueprintAssignable)
	FOnMoodChanged OnMoodChanged;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	TObjectPtr<UDataTable> PersonalityTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	TObjectPtr<UDataTable> JobTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	TObjectPtr<UDataTable> ActivityTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evaluation")
	int32 AgeUpdateIntervalHours = 6;
	
private:
	int32 CurrentHourTillAgeUpdate;
	EMoodState PreviousMoodState;
	EHealthState PreviousHealthState;
	
	void EvaluateNPC(ANPCCharacter* NPC, int32 CurrentHour);
	static bool IsWorkingHour(const FNPCJobPool& JobData, int32 CurrentHour);
	void EvaluateMoodChange(const EMoodState& NewMoodState, int32 NPC_ID);
	void EvaluateHealthChange(const EHealthState& NewHealthState, int32 NPC_ID);

	static float CalculateMoodChange(const FNPCState& State, const FPersonalityTemplate& Personality, const FNPCJobPool& Job, const FNPCActivity& Activity);
	static float CalculateHealthChange(const FNPCState& State, const FNPCJobPool& Job, const FNPCActivity& Activity);

};
