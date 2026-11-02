// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ANPCGameMode.generated.h"

class UTimeManagementComponent;
/**
 * 
 */
UCLASS()
class PCGARTEFACT_API ANPCGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:

	ANPCGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UTimeManagementComponent> TimeManagement;
};
