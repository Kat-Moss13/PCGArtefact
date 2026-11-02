// Fill out your copyright notice in the Description page of Project Settings.


#include "ANPCGameMode.h"

#include "UTimeManagementComponent.h"

ANPCGameMode::ANPCGameMode()
{
	TimeManagement = CreateDefaultSubobject<UTimeManagementComponent>(
		TEXT("TimeManagement"));
}
