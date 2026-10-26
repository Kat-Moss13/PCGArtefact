// Fill out your copyright notice in the Description page of Project Settings.


#include "UWeightedRandomLibrary.h"

float UWeightedRandomLibrary::GetWeightedRandomFloat(const TArray<FWeightedRange>& Ranges, FRandomStream& RandomStream)
{
	if (Ranges.IsEmpty()) return 0.f;

	float TotalWeight = 0.f;

	for (const FWeightedRange& Range : Ranges)
	{
		TotalWeight += Range.Weight;
	}

	if (TotalWeight <= 0.f) return 0.f;

	const float Roll = RandomStream.FRandRange(0.f, TotalWeight);

	float RunningWeight = 0.f;

	for (const FWeightedRange& Range : Ranges)
	{
		RunningWeight += Range.Weight;

		if (Roll <= RunningWeight)
		{
			return RandomStream.FRandRange(Range.Min, Range.Max);
		}
	}

	return FMath::RandRange(
		Ranges.Last().Min,
		Ranges.Last().Max);
}
