// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Metadata/PCGMetadata.h"
#include "Metadata/PCGMetadataAttributeTpl.h"
#include "FNPCMetadataAttributes.generated.h"

USTRUCT(BlueprintType)
struct FNPCMetadataAttributes
{
	GENERATED_BODY();
	
	const FPCGMetadataAttribute<FString>* FirstName = nullptr;
	const FPCGMetadataAttribute<FString>* Surname = nullptr;
	const FPCGMetadataAttribute<bool>* Sex = nullptr;
	const FPCGMetadataAttribute<int64>* Personality = nullptr;
	const FPCGMetadataAttribute<int64>* Job = nullptr;

	static FNPCMetadataAttributes Create(const UPCGMetadata* Metadata)
	{
		FNPCMetadataAttributes Attributes;

		if (!Metadata)
		{
			return Attributes;
		}

		Attributes.FirstName = Metadata->GetConstTypedAttribute<FString>(TEXT("FirstName"));
		Attributes.Surname = Metadata->GetConstTypedAttribute<FString>(TEXT("Surname"));
		Attributes.Sex = Metadata->GetConstTypedAttribute<bool>(TEXT("Sex"));
		Attributes.Personality = Metadata->GetConstTypedAttribute<int64>(TEXT("PersonalityType"));
		Attributes.Job = Metadata->GetConstTypedAttribute<int64>(TEXT("JobType"));

		return Attributes;
	}
private:
	static FNPCMetadataAttributes NPCMetadataAttributes;
};
