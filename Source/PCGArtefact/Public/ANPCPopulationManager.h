// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "INPCPopulationManagerInterface.h"
#include "UNPCStateEvaluatorComponent.h"
#include "GameFramework/Actor.h"
#include "ANPCPopulationManager.generated.h"

UCLASS()
class PCGARTEFACT_API ANPCPopulationManager : public AActor, public INPCPopulationManagerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
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

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Population", meta=(AllowPrivateAccess="true"))
	TMap<int32, TObjectPtr<ANPCCharacter>> NPCRegistry;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Population", meta=(AllowPrivateAccess="true"))
	TArray<int32> RegisteredIDs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Population", meta=(AllowPrivateAccess="true"))
	int32 RegisteredNPCCount = 0;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNPCStateEvaluatorComponent> StateEvaluator;
};
