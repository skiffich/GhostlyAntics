// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IteractibleItemUI.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API UIteractibleItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateInteractionValue(float Value);
};
