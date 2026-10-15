// LSystemTreeActor.h
// Week 3 Tutorial 1: L-System Engine (Part E: seeing it in Unreal Engine 5.8)
//
// A thin Unreal wrapper around the engine-agnostic L-system core.
// Rewriting and turtle interpretation run in plain C++ (tested in Visual Studio);
// this actor only exposes settings and turns segments into cylinder instances.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSystemTreeActor.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;
class UStaticMesh;

UENUM(BlueprintType)
enum class ELSystemPreset : uint8
{
	BracketTree,
	FractalPlant,
	StochasticBush,
	MyPlant,
	Custom UMETA(DisplayName = "Custom (rules below)")
};

// One production, editable in the Details panel.
USTRUCT(BlueprintType)
struct FLSystemRule
{
	GENERATED_BODY()

	// Only the first character is used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L-System")
	FString Predecessor = TEXT("F");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L-System")
	FString Successor = TEXT("F[+F]F[-F]F");

	// Used only when a predecessor has several rules; normalised automatically.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "L-System", meta = (ClampMin = "0.0"))
	double Probability = 1.0;
};

UCLASS()
class ALSystemTreeActor : public AActor
{
	GENERATED_BODY()

public:
	ALSystemTreeActor();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "L-System")
	void Generate();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "L-System")
	void ClearTree();

protected:
	virtual void BeginPlay() override;

	// ---------------------------------------------------------------- Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L-System|Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "L-System|Components")
	TObjectPtr<UInstancedStaticMeshComponent> BranchInstances;

	// ---------------------------------------------------------------- Rules

	UPROPERTY(EditAnywhere, Category = "L-System|Rules")
	ELSystemPreset Preset = ELSystemPreset::StochasticBush;

	UPROPERTY(EditAnywhere, Category = "L-System|Rules")
	int32 Seed = 42;

	// If ticked, Iterations replaces the preset's own value.
	UPROPERTY(EditAnywhere, Category = "L-System|Rules", meta = (InlineEditConditionToggle))
	bool bOverrideIterations = false;

	UPROPERTY(EditAnywhere, Category = "L-System|Rules", meta = (ClampMin = "0", ClampMax = "10", EditCondition = "bOverrideIterations"))
	int32 Iterations = 5;

	UPROPERTY(EditAnywhere, Category = "L-System|Custom", meta = (EditCondition = "Preset == ELSystemPreset::Custom", EditConditionHides))
	FString Axiom = TEXT("F");

	UPROPERTY(EditAnywhere, Category = "L-System|Custom", meta = (EditCondition = "Preset == ELSystemPreset::Custom", EditConditionHides))
	TArray<FLSystemRule> Rules;

	UPROPERTY(EditAnywhere, Category = "L-System|Custom", meta = (ClampMin = "0.0", ClampMax = "180.0",
		EditCondition = "Preset == ELSystemPreset::Custom", EditConditionHides))
	double AngleDegrees = 25.0;

	UPROPERTY(EditAnywhere, Category = "L-System|Custom", meta = (ClampMin = "0.1", ClampMax = "1.0",
		EditCondition = "Preset == ELSystemPreset::Custom", EditConditionHides))
	double LengthScalePerDepth = 1.0;

	// ---------------------------------------------------------------- Appearance

	// Use a 100-unit cylinder with a centred pivot, e.g. /Engine/BasicShapes/Cylinder.
	UPROPERTY(EditAnywhere, Category = "L-System|Appearance")
	TObjectPtr<UStaticMesh> BranchMesh;

	UPROPERTY(EditAnywhere, Category = "L-System|Appearance", meta = (ClampMin = "1.0", Units = "cm"))
	double SegmentLength = 30.0;

	UPROPERTY(EditAnywhere, Category = "L-System|Appearance", meta = (ClampMin = "0.1", Units = "cm"))
	double TrunkRadius = 8.0;

	UPROPERTY(EditAnywhere, Category = "L-System|Appearance", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	double RadiusScalePerDepth = 0.65;

	UPROPERTY(EditAnywhere, Category = "L-System|Appearance", meta = (ClampMin = "0.1", Units = "cm"))
	double MinimumRadius = 0.75;

	// ---------------------------------------------------------------- Safety

	UPROPERTY(EditAnywhere, Category = "L-System|Safety", meta = (ClampMin = "1000"))
	int32 MaxSymbols = 2000000;

	UPROPERTY(EditAnywhere, Category = "L-System|Safety", meta = (ClampMin = "100"))
	int32 MaxInstances = 50000;

	UPROPERTY(EditAnywhere, Category = "L-System|Safety")
	bool bGenerateOnBeginPlay = true;
};
