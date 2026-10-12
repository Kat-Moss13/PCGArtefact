// CaveGeneratorActor.cpp
// Week 2 Tutorial 2: Hybrid Noise + CA (Part E: playtesting in Unreal Engine 5.8)

#include "CaveGeneratorActor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"

// Engine-agnostic core, copied into this module from the Visual Studio project.
#include "CaveAnalysis.h"
#include "CellularAutomaton.h"

#include <cstdint>

ACaveGeneratorActor::ACaveGeneratorActor()
{
	// Generation happens on demand, so this actor never needs to tick.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	// One instanced component draws every wall cell in very few draw calls,
	// instead of spawning thousands of separate actors.
	WallInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallInstances"));
	WallInstances->SetupAttachment(SceneRoot);
}

void ACaveGeneratorActor::BeginPlay()
{
	Super::BeginPlay();

	if (bGenerateOnBeginPlay)
	{
		Generate();
	}
}

void ACaveGeneratorActor::ClearCave()
{
	if (WallInstances)
	{
		WallInstances->ClearInstances();
	}
}

void ACaveGeneratorActor::Generate()
{
	if (!WallInstances)
	{
		return;
	}

	if (!WallMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: assign a Wall Mesh before generating."), *GetName());
		return;
	}

	// Record an undo transaction and mark the level dirty when used from the editor button.
	Modify();
	WallInstances->Modify();

	WallInstances->SetStaticMesh(WallMesh);
	WallInstances->ClearInstances();

	// ---- 1. Generate the grid with exactly the same code as the Visual Studio harness.
	// Reinterpret the (signed) int32 seed as unsigned so negative seeds are valid and stable.
	const std::uint64_t CaveSeed = static_cast<std::uint64_t>(static_cast<uint32>(Seed));

	HybridCave::CaveGrid Grid;
	if (Strategy == ECaveSeedStrategy::Uniform)
	{
		Grid = HybridCave::InitialiseUniform(GridWidth, GridHeight, CaveSeed, UniformWallProbability);
	}
	else
	{
		HybridCave::NoiseSeedSettings Noise;
		Noise.Fbm.Frequency = NoiseFrequency;
		Noise.Fbm.Octaves   = NoiseOctaves;
		Noise.Bias          = NoiseBias;
		Noise.Contrast      = NoiseContrast;
		Grid = HybridCave::InitialiseFromNoise(GridWidth, GridHeight, CaveSeed, Noise);
	}

	HybridCave::CaSettings Automaton;
	Automaton.MaxPasses = MaxPasses;
	HybridCave::RunMajorityPasses(Grid, Automaton);

	int32 FloorCulled = 0;
	if (bCullDisconnectedRegions)
	{
		const HybridCave::RegionLabels Regions = HybridCave::LabelFloorRegions(Grid);
		FloorCulled = HybridCave::CullToLargestRegion(Grid, Regions);
	}

	// ---- 2. Convert wall cells into instance transforms.
	TArray<FTransform> Transforms;
	Transforms.Reserve(Grid.CellCount());

	// The engine cube is 100 units across with a centred pivot.
	const FVector InstanceScale(CellSize / 100.0, CellSize / 100.0, WallHeight / 100.0);

	for (int32 Y = 0; Y < Grid.Height; ++Y)
	{
		for (int32 X = 0; X < Grid.Width; ++X)
		{
			if (Grid.Get(X, Y) == HybridCave::Wall)
			{
				const FVector Location((X + 0.5) * CellSize, (Y + 0.5) * CellSize, WallHeight * 0.5);
				Transforms.Emplace(FQuat::Identity, Location, InstanceScale);
			}
		}
	}

	// Add in one call: far cheaper than calling AddInstance once per wall.
	WallInstances->AddInstances(Transforms, /*bShouldReturnIndices*/ false);

	UE_LOG(LogTemp, Log, TEXT("%s: seed %d (%s) produced %d wall instances; %d floor cells culled."),
		*GetName(), Seed,
		Strategy == ECaveSeedStrategy::Uniform ? TEXT("Uniform") : TEXT("NoiseSeeded"),
		Transforms.Num(), FloorCulled);
}
