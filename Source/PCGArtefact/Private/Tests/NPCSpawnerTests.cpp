#include "Misc/AutomationTest.h"
#include "ANPCSpawner.h"

namespace NPCSpawnerTestHelpers
{
	static TArray<FName> BuildPool(const int32 PoolSize)
	{
		TArray<FName> Pool;

		for (int32 i = 1; i <= PoolSize; ++i)
		{
			Pool.Add(FName(*FString::FromInt(i)));
		}

		return Pool;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FUniqueNamesReturnsRequestedCount,
	"PCGArtefact.NPCSpawner.UniqueNames.ReturnsRequestedCount",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUniqueNamesReturnsRequestedCount::RunTest(const FString& Parameters)
{
	const TArray<int32> PoolSizes =
	{
		1,
		5,
		10,
		25,
		50,
		100,
		500
	};

	for (const int32 PoolSize : PoolSizes)
	{
		for (int32 Seed = 0; Seed < 100; ++Seed)
		{
			const TArray<FName> Pool =
				NPCSpawnerTestHelpers::BuildPool(PoolSize);

			const int32 RequestedCount =
				FMath::Max(1, PoolSize / 2);

			const TArray<FName> Result =
				ANPCSpawner::GetUniqueNamesFromPool(
					Pool,
					RequestedCount,
					Seed);

			TestEqual(
				FString::Printf(
					TEXT("Pool=%d Seed=%d"),
					PoolSize,
					Seed),
				Result.Num(),
				RequestedCount);
		}
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FUniqueNamesNoDuplicates,
	"PCGArtefact.NPCSpawner.UniqueNames.NoDuplicates",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUniqueNamesNoDuplicates::RunTest(const FString& Parameters)
{
	const TArray<int32> PoolSizes =
	{
		10,
		25,
		50,
		100,
		500
	};

	for (const int32 PoolSize : PoolSizes)
	{
		for (int32 Seed = 0; Seed < 1000; ++Seed)
		{
			const TArray<FName> Result =
				ANPCSpawner::GetUniqueNamesFromPool(
					NPCSpawnerTestHelpers::BuildPool(PoolSize),
					FMath::Max(1, PoolSize / 2),
					Seed);

			const TSet<FName> UniqueNames(Result);
			
			TestEqual(
				FString::Printf(
					TEXT("Pool=%d Seed=%d"),
					PoolSize,
					Seed),
				UniqueNames.Num(),
				Result.Num());
		}
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FUniqueNamesDeterministicSeed,
	"PCGArtefact.NPCSpawner.UniqueNames.DeterministicSeed",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUniqueNamesDeterministicSeed::RunTest(const FString& Parameters)
{
	const TArray<int32> PoolSizes =
	{
		10,
		25,
		50,
		100,
		500
	};

	const TArray<int32> RequestCounts =
	{
		1,
		5,
		10,
		20,
		50
	};

	for (const int32 PoolSize : PoolSizes)
	{
		for (const int32 RequestCount : RequestCounts)
		{
			if (RequestCount > PoolSize)
			{
				continue;
			}

			for (int32 Seed = 0; Seed < 1000; ++Seed)
			{
				const TArray<FName> Pool =
					NPCSpawnerTestHelpers::BuildPool(PoolSize);

				const TArray<FName> ResultA =
					ANPCSpawner::GetUniqueNamesFromPool(
						Pool,
						RequestCount,
						Seed);

				const TArray<FName> ResultB =
					ANPCSpawner::GetUniqueNamesFromPool(
						Pool,
						RequestCount,
						Seed);

				TestTrue(
					FString::Printf(
						TEXT("Pool=%d Request=%d Seed=%d"),
						PoolSize,
						RequestCount,
						Seed),
					ResultA == ResultB);
			}
		}
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FUniqueNamesDifferentSeeds,
	"PCGArtefact.NPCSpawner.UniqueNames.DifferentSeeds",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUniqueNamesDifferentSeeds::RunTest(const FString& Parameters)
{
	int32 DifferentResults = 0;

	for (int32 Seed = 0; Seed < 1000; ++Seed)
	{
		constexpr int32 PoolSize = 100;
		constexpr int32 RequestCount = 20;
		const TArray<FName> Pool =
			NPCSpawnerTestHelpers::BuildPool(PoolSize);

		const TArray<FName> ResultA =
			ANPCSpawner::GetUniqueNamesFromPool(
				Pool,
				RequestCount,
				Seed);

		const TArray<FName> ResultB =
			ANPCSpawner::GetUniqueNamesFromPool(
				Pool,
				RequestCount,
				Seed + 1);

		if (ResultA != ResultB)
		{
			++DifferentResults;
		}
	}

	TestTrue(
		TEXT("Majority of seeds produce different outputs"),
		DifferentResults > 950);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FUniqueNamesRequestMoreThanPool,
	"PCGArtefact.NPCSpawner.UniqueNames.RequestMoreThanPool",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUniqueNamesRequestMoreThanPool::RunTest(const FString& Parameters)
{
	const TArray<int32> PoolSizes =
	{
		1,
		5,
		10,
		25,
		50,
		100,
		500
	};

	for (const int32 PoolSize : PoolSizes)
	{
		for (int32 Seed = 0; Seed < 100; ++Seed)
		{
			const TArray<FName> Result =
				ANPCSpawner::GetUniqueNamesFromPool(
					NPCSpawnerTestHelpers::BuildPool(PoolSize),
					PoolSize * 10,
					Seed);

			TestEqual(
				FString::Printf(
					TEXT("Pool=%d Seed=%d"),
					PoolSize,
					Seed),
				Result.Num(),
				PoolSize);
		}
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FUniqueNamesEmptyPool,
	"PCGArtefact.NPCSpawner.UniqueNames.EmptyPool",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUniqueNamesEmptyPool::RunTest(const FString& Parameters)
{
	for (int32 Seed = 0; Seed < 1000; ++Seed)
	{
		TArray<FName> EmptyPool;

		const TArray<FName> Result =
			ANPCSpawner::GetUniqueNamesFromPool(
				EmptyPool,
				100,
				Seed);

		TestEqual(
			FString::Printf(
				TEXT("Empty Pool Seed=%d"),
				Seed),
			Result.Num(),
			0);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FUniqueNamesSeedStress,
	"PCGArtefact.NPCSpawner.UniqueNames.SeedStress",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUniqueNamesSeedStress::RunTest(const FString& Parameters)
{
	const TArray<FName> Pool =
		NPCSpawnerTestHelpers::BuildPool(100);

	for (int32 Seed = 0; Seed < 10000; ++Seed)
	{
		const TArray<FName> Result =
			ANPCSpawner::GetUniqueNamesFromPool(
				Pool,
				20,
				Seed);

		TestEqual(
			FString::Printf(TEXT("Count_%d"), Seed),
			Result.Num(),
			20);

		const TSet<FName> UniqueNames(Result);

		TestEqual(
			FString::Printf(TEXT("Unique_%d"), Seed),
			UniqueNames.Num(),
			Result.Num());
	}

	return true;
}