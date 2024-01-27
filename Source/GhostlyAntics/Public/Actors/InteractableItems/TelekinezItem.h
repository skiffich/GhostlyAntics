// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableItems/InteractableItem.h"
#include "TelekinezItem.generated.h"

UENUM(BlueprintType)
enum class EInteractTelekinezType : uint8
{
	None,
	TelekinezThrowUp,
	TelekinezThrowDown,
	TelekinezMove
};

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API ATelekinezItem : public AInteractableItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EInteractTelekinezType InteractTelekinezType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	bool ShouldResetAfterInteraction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float PhysicsSimPeriod;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float PhysicsForceModifier;

public:
	ATelekinezItem();

protected:
	virtual void Interact() override;

private:
	void TelekinezThrowUp();

	void TelekinezThrowParabolic();

	void TelekinezMove();

	void TelekinezInit();

	void TelekinezFinish();

private:
	FVector PrevLocation;
	FRotator PrevRotation;
};
