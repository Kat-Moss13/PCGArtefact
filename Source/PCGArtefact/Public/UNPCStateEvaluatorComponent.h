// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UNPCStateEvaluatorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PCGARTEFACT_API UNPCStateEvaluatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNPCStateEvaluatorComponent();

	void EvaluateAllNPCs(TArray<ANPCCharacter*> NPCs);
	
	
protected:
	virtual void BeginPlay() override;

};
