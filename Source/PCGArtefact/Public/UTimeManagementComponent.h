#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UTimeManagementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourChanged, int32, NewHour);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PCGARTEFACT_API UTimeManagementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTimeManagementComponent();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category ="Time")
	FOnHourChanged OnHourChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Time")
	int32 GetCurrentHour() const {return CurrentHour;}
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
	float SecondsPerHour = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
	int32 CurrentHour;
	
private:
	
	FTimerHandle TimerHandle;
	UFUNCTION()
	void AdvanceHour();
};
