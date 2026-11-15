#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidgetComponents.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UNumericString.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PCGARTEFACT_API UUNumericString : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	static bool AreEditorUtilityInputsNumeric(TSet<UEditorUtilityEditableTextBox*> Inputs);
};
