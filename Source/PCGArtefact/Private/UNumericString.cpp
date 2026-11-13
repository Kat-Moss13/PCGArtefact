// Fill out your copyright notice in the Description page of Project Settings.


#include "UNumericString.h"

bool UUNumericString::AreEditorUtilityInputsNumeric(TSet<UEditorUtilityEditableTextBox*> Inputs)
{
	for (UEditorUtilityEditableTextBox* Input : Inputs)
	{
		if (!Input->GetText().ToString().IsNumeric())
		{
			UE_LOG(LogTemp, Warning, TEXT("There is a non number. Why?"));
			return false;
		}
	}
	return true;
}
