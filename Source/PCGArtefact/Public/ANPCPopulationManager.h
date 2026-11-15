#pragma once

#include "CoreMinimal.h"
#include "ANPCSpawner.h"
#include "INPCPopulationManagerInterface.h"
#include "UNPCStateEvaluatorComponent.h"
#include "GameFramework/Actor.h"
#include "ANPCPopulationManager.generated.h"

UCLASS()
class PCGARTEFACT_API ANPCPopulationManager : public AActor, public INPCPopulationManagerInterface
{
	GENERATED_BODY()

public:
	ANPCPopulationManager();

	UFUNCTION(BlueprintCallable)
	virtual void RegisterNPCs_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetNPCCount_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	virtual ANPCCharacter*  GetNPCByID_Implementation(int32 NPC_ID) __override;

	UFUNCTION(BlueprintCallable)
	virtual TArray<ANPCCharacter*> GetNPCs_Implementation() __override;

	UFUNCTION(BlueprintCallable)
	void HandleHourChanged(const int32 CurrentHour);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleMoodChanged(ANPCCharacter* NPC, EMoodState Mood);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleHealthChanged(ANPCCharacter* NPC, EHealthState Health);
	
protected:

	virtual void BeginPlay() override;

private:
	
	UFUNCTION()
	void HandleMoodUpdated(EMoodState Mood, int32 NPC_ID);

	UFUNCTION()
	void HandleHealthUpdated(EHealthState Health, int32 NPC_ID);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Population", meta=(AllowPrivateAccess="true"))
	TMap<int32, TObjectPtr<ANPCCharacter>> NPCRegistry;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Population", meta=(AllowPrivateAccess="true"))
	TArray<int32> RegisteredIDs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Population", meta=(AllowPrivateAccess="true"))
	int32 RegisteredNPCCount = 0;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNPCStateEvaluatorComponent> StateEvaluator;

	UPROPERTY(VisibleAnywhere)
	ANPCSpawner* Spawner;
};
