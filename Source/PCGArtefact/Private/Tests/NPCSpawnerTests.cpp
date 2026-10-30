#include "Misc/AutomationTest.h"
#include "ANPCSpawner.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUniqueNamesReturnsRequestedCount,
    "PCGArtefact.NPCSpawner.UniqueNames.ReturnsRequestedCount",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUniqueNamesNoDuplicates,
    "PCGArtefact.NPCSpawner.UniqueNames.NoDuplicates",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUniqueNamesDeterministicSeed,
    "PCGArtefact.NPCSpawner.UniqueNames.DeterministicSeed",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUniqueNamesDifferentSeeds,
    "PCGArtefact.NPCSpawner.UniqueNames.DifferentSeeds",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUniqueNamesRequestMoreThanPool,
    "PCGArtefact.NPCSpawner.UniqueNames.RequestMoreThanPool",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUniqueNamesEmptyPool,
    "PCGArtefact.NPCSpawner.UniqueNames.EmptyPool",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUniqueNamesReturnsRequestedCount::RunTest(const FString& Parameters)
{
    TArray<FName> Pool;

    for (int32 i = 1; i <= 100; ++i)
    {
        Pool.Add(FName(*FString::FromInt(i)));
    }

    const TArray<FName> Result = ANPCSpawner::GetUniqueNamesFromPool(Pool, 20, 1234);

    TestEqual(TEXT("Returns requested count"), Result.Num(), 20);

    return true;
}

bool FUniqueNamesNoDuplicates::RunTest(const FString& Parameters)
{
    TArray<FName> Pool;

    for (int32 i = 1; i <= 100; ++i)
    {
        Pool.Add(FName(*FString::FromInt(i)));
    }

    const TArray<FName> Result = ANPCSpawner::GetUniqueNamesFromPool(Pool, 50, 1234);

    TSet<FName> UniqueNames(Result);

    TestEqual(TEXT("All names are unique"), UniqueNames.Num(), Result.Num());

    return true;
}

bool FUniqueNamesDeterministicSeed::RunTest(const FString& Parameters)
{
    TArray<FName> Pool;

    for (int32 i = 1; i <= 100; ++i)
    {
        Pool.Add(FName(*FString::FromInt(i)));
    }

    const TArray<FName> ResultA = ANPCSpawner::GetUniqueNamesFromPool(Pool, 20, 1234);

    const TArray<FName> ResultB = ANPCSpawner::GetUniqueNamesFromPool(Pool, 20, 1234);

    TestEqual(TEXT("Same seed returns same result"), ResultA, ResultB);

    return true;
}

bool FUniqueNamesDifferentSeeds::RunTest(const FString& Parameters)
{
    TArray<FName> Pool;

    for (int32 i = 1; i <= 100; ++i)
    {
        Pool.Add(FName(*FString::FromInt(i)));
    }

    const TArray<FName> ResultA = ANPCSpawner::GetUniqueNamesFromPool(Pool, 20, 1234);

    const TArray<FName> ResultB = ANPCSpawner::GetUniqueNamesFromPool(Pool, 20, 5678);

    TestNotEqual(TEXT("Different seeds return different result"), ResultA, ResultB);

    return true;
}

bool FUniqueNamesRequestMoreThanPool::RunTest(const FString& Parameters)
{
    TArray<FName> Pool;

    for (int32 i = 1; i <= 100; ++i)
    {
        Pool.Add(FName(*FString::FromInt(i)));
    }

    const TArray<FName> Result = ANPCSpawner::GetUniqueNamesFromPool(Pool, 200, 1234);

    TestEqual(TEXT("Cannot exceed pool size"), Result.Num(), 100);

    return true;
}

bool FUniqueNamesEmptyPool::RunTest(const FString& Parameters)
{
    TArray<FName> Pool;

    const TArray<FName> Result = ANPCSpawner::GetUniqueNamesFromPool(Pool, 20, 1234);

    TestEqual(TEXT("Empty pool returns empty result"), Result.Num(), 0);

    return true;
}