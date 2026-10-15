// LSystemTreeActor.cpp
// Week 3 Tutorial 1: L-System Engine (Part E: seeing it in Unreal Engine 5.8)

#include "LSystemTreeActor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"

// Engine-agnostic core, copied into this module from the Visual Studio project.
#include "LSystem.h"
#include "Presets.h"
#include "Turtle.h"

#include <cmath>
#include <cstdint>
#include <string>

namespace
{
	LSystem::LSystemDefinition BuildDefinition(ELSystemPreset Preset, const FString& Axiom,
		const TArray<FLSystemRule>& Rules, double AngleDegrees, double LengthScalePerDepth)
	{
		switch (Preset)
		{
			case ELSystemPreset::BracketTree:    return LSystem::Presets::BracketTree();
			case ELSystemPreset::FractalPlant:   return LSystem::Presets::FractalPlant();
			case ELSystemPreset::StochasticBush: return LSystem::Presets::StochasticBush();
			case ELSystemPreset::MyPlant:        return LSystem::Presets::MyPlant();
			default: break;
		}

		// Custom: convert Unreal strings (UTF-16 TCHAR) into the core's std::string.
		LSystem::LSystemDefinition Definition;
		Definition.Name = "Custom";
		Definition.Axiom = TCHAR_TO_UTF8(*Axiom);
		Definition.AngleDegrees = AngleDegrees;
		Definition.LengthScalePerDepth = LengthScalePerDepth;

		for (const FLSystemRule& Rule : Rules)
		{
			if (Rule.Predecessor.IsEmpty())
			{
				continue;
			}
			const char Predecessor = static_cast<char>(Rule.Predecessor[0]);
			Definition.Rules.Add(Predecessor, std::string(TCHAR_TO_UTF8(*Rule.Successor)), Rule.Probability);
		}
		Definition.Rules.NormaliseProbabilities();
		return Definition;
	}
}

ALSystemTreeActor::ALSystemTreeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BranchInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BranchInstances"));
	BranchInstances->SetupAttachment(SceneRoot);

	// A sensible custom rule so the Custom preset works immediately.
	Rules.Add(FLSystemRule());
}

void ALSystemTreeActor::BeginPlay()
{
	Super::BeginPlay();

	if (bGenerateOnBeginPlay)
	{
		Generate();
	}
}

void ALSystemTreeActor::ClearTree()
{
	if (BranchInstances)
	{
		BranchInstances->ClearInstances();
	}
}

void ALSystemTreeActor::Generate()
{
	if (!BranchInstances)
	{
		return;
	}
	if (!BranchMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: assign a Branch Mesh before generating."), *GetName());
		return;
	}

	Modify();
	BranchInstances->Modify();
	BranchInstances->SetStaticMesh(BranchMesh);
	BranchInstances->ClearInstances();

	// ---- 1. Rewrite: exactly the same code as the Visual Studio project.
	LSystem::LSystemDefinition Definition = BuildDefinition(Preset, Axiom, Rules, AngleDegrees, LengthScalePerDepth);
	if (bOverrideIterations)
	{
		Definition.Iterations = Iterations;
	}

	LSystem::GenerationSettings Settings;
	Settings.Seed = static_cast<std::uint64_t>(static_cast<uint32>(Seed));
	Settings.MaxSymbols = static_cast<std::size_t>(MaxSymbols);

	const LSystem::GenerationResult Generation = LSystem::Generate(Definition, Settings);
	if (!Generation.Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *GetName(), UTF8_TO_TCHAR(Generation.Error.c_str()));
		return;
	}

	std::string BracketError;
	if (!LSystem::ValidateBrackets(Generation.Symbols, BracketError))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: bracket error: %s"), *GetName(), UTF8_TO_TCHAR(BracketError.c_str()));
		return;
	}

	// ---- 2. Interpret with a unit step; SegmentLength converts to centimetres below.
	LSystem::TurtleSettings Turtle;
	Turtle.StepLength            = 1.0;
	Turtle.AngleDegrees          = Definition.AngleDegrees;
	Turtle.InitialHeadingDegrees = Definition.InitialHeadingDegrees;
	Turtle.LengthScalePerDepth   = Definition.LengthScalePerDepth;
	Turtle.DrawSymbols           = Definition.DrawSymbols;
	Turtle.MoveSymbols           = Definition.MoveSymbols;

	const LSystem::TurtleResult Result = LSystem::InterpretTurtle(Generation.Symbols, Turtle);
	if (!Result.Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: turtle error: %s"), *GetName(), UTF8_TO_TCHAR(Result.Error.c_str()));
		return;
	}

	const int32 SegmentCount = static_cast<int32>(Result.Segments.size());
	if (SegmentCount > MaxInstances)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: %d segments exceeds MaxInstances (%d). Reduce Iterations."),
			*GetName(), SegmentCount, MaxInstances);
		return;
	}

	// ---- 3. Realise: one cylinder instance per segment.
	TArray<FTransform> Transforms;
	Transforms.Reserve(SegmentCount);

	for (const LSystem::Segment& Seg : Result.Segments)
	{
		// The turtle works in 2D (X across, Y up). Map it onto the actor's XZ plane (Z is up in Unreal).
		const FVector Start(Seg.Start.X * SegmentLength, 0.0, Seg.Start.Y * SegmentLength);
		const FVector End(Seg.End.X * SegmentLength, 0.0, Seg.End.Y * SegmentLength);

		const FVector Direction = End - Start;
		const double Length = Direction.Size();
		if (Length < KINDA_SMALL_NUMBER)
		{
			continue;
		}

		// Branches thin with depth, but never below MinimumRadius.
		const double Radius = FMath::Max(MinimumRadius, TrunkRadius * std::pow(RadiusScalePerDepth, static_cast<double>(Seg.Depth)));

		// The engine cylinder is 100 units tall along its local Z axis with a centred pivot:
		// rotate Z onto the segment, place it at the midpoint, and scale it to fit.
		const FQuat Rotation = FRotationMatrix::MakeFromZ(Direction / Length).ToQuat();
		const FVector Location = (Start + End) * 0.5;
		const FVector Scale(Radius * 2.0 / 100.0, Radius * 2.0 / 100.0, Length / 100.0);

		Transforms.Emplace(Rotation, Location, Scale);
	}

	BranchInstances->AddInstances(Transforms, /*bShouldReturnIndices*/ false);

	UE_LOG(LogTemp, Log, TEXT("%s: %s, seed %d, %d iterations: %d symbols, %d segments, max depth %d."),
		*GetName(), UTF8_TO_TCHAR(Definition.Name.c_str()), Seed, Generation.IterationsCompleted,
		static_cast<int32>(Generation.Symbols.size()), Transforms.Num(), Result.MaxDepth);
}
