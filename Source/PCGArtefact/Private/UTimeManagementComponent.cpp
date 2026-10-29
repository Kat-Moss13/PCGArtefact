#include "UTimeManagementComponent.h"


UTimeManagementComponent::UTimeManagementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UTimeManagementComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTimeManagementComponent::AdvanceHour, SecondsPerHour, true );
	
}

void UTimeManagementComponent::AdvanceHour()
{
	CurrentHour += 1;
	
	if (CurrentHour >= 24)
	{
		CurrentHour = 0;
	}
	
	OnHourChanged.Broadcast(CurrentHour);
}

