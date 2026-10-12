// CaveGeneratorActor.h
// Week 2 Tutorial 2: Hybrid Noise + CA (Part E: playtesting in Unreal Engine 5.8)
//
// A thin Unreal wrapper around the engine-agnostic HybridCave core.
// All generation logic lives in plain C++ (tested in Visual Studio first);
// this actor only exposes parameters to the Details panel and turns wall
// cells into instanced static meshes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaveGeneratorActor.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;
class UStaticMesh;

UENUM(BlueprintType)
enum class ECaveSeedStrategy : uint8
{
	Uniform     UMETA(DisplayName = "Uniform (CA only)"),
	NoiseSeeded UMETA(DisplayName = "Noise-seeded CA")
};

UCLASS()
class ACaveGeneratorActor : public AActor
{
	GENERATED_BODY()

public:
	ACaveGeneratorActor();

	// Regenerates the cave. CallInEditor adds a button to the Details panel.
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Cave")
	void Generate();

	// Removes all wall instances.
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Cave")
	void ClearCave();

protected:
	virtual void BeginPlay() override;

	// ---------------------------------------------------------------- Components
	// TObjectPtr + UPROPERTY: the garbage collector can see and track these references.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cave|Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cave|Components")
	TObjectPtr<UInstancedStaticMeshComponent> WallInstances;

	// ---------------------------------------------------------------- Setup

	// Use a 100 x 100 x 100 unit cube with a centred pivot, e.g. /Engine/BasicShapes/Cube.
	UPROPERTY(EditAnywhere, Category = "Cave|Setup")
	TObjectPtr<UStaticMesh> WallMesh;

	UPROPERTY(EditAnywhere, Category = "Cave|Setup", meta = (ClampMin = "10.0", Units = "cm"))
	double CellSize = 100.0;

	UPROPERTY(EditAnywhere, Category = "Cave|Setup", meta = (ClampMin = "10.0", Units = "cm"))
	double WallHeight = 300.0;

	UPROPERTY(EditAnywhere, Category = "Cave|Setup")
	bool bGenerateOnBeginPlay = true;

	// ---------------------------------------------------------------- Generation

	UPROPERTY(EditAnywhere, Category = "Cave|Generation")
	int32 Seed = 1000;

	UPROPERTY(EditAnywhere, Category = "Cave|Generation")
	ECaveSeedStrategy Strategy = ECaveSeedStrategy::NoiseSeeded;

	UPROPERTY(EditAnywhere, Category = "Cave|Generation", meta = (ClampMin = "8", ClampMax = "512"))
	int32 GridWidth = 96;

	UPROPERTY(EditAnywhere, Category = "Cave|Generation", meta = (ClampMin = "8", ClampMax = "512"))
	int32 GridHeight = 72;

	UPROPERTY(EditAnywhere, Category = "Cave|Generation", meta = (ClampMin = "0", ClampMax = "20"))
	int32 MaxPasses = 5;

	UPROPERTY(EditAnywhere, Category = "Cave|Generation")
	bool bCullDisconnectedRegions = true;

	UPROPERTY(EditAnywhere, Category = "Cave|Uniform", meta = (ClampMin = "0.0", ClampMax = "1.0",
		EditCondition = "Strategy == ECaveSeedStrategy::Uniform", EditConditionHides))
	double UniformWallProbability = 0.45;

	UPROPERTY(EditAnywhere, Category = "Cave|Noise", meta = (ClampMin = "0.001", ClampMax = "1.0",
		EditCondition = "Strategy == ECaveSeedStrategy::NoiseSeeded", EditConditionHides))
	double NoiseFrequency = 0.05;

	UPROPERTY(EditAnywhere, Category = "Cave|Noise", meta = (ClampMin = "1", ClampMax = "8",
		EditCondition = "Strategy == ECaveSeedStrategy::NoiseSeeded", EditConditionHides))
	int32 NoiseOctaves = 3;

	UPROPERTY(EditAnywhere, Category = "Cave|Noise", meta = (ClampMin = "0.0", ClampMax = "1.0",
		EditCondition = "Strategy == ECaveSeedStrategy::NoiseSeeded", EditConditionHides))
	double NoiseBias = 0.45;

	UPROPERTY(EditAnywhere, Category = "Cave|Noise", meta = (ClampMin = "0.0", ClampMax = "3.0",
		EditCondition = "Strategy == ECaveSeedStrategy::NoiseSeeded", EditConditionHides))
	double NoiseContrast = 0.9;
};
