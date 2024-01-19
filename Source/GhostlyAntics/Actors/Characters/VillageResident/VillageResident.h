// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Characters/GACharacter/GACharacter.h"
#include "VillageResident.generated.h"

UENUM(BlueprintType)
enum class EVillagerState : uint8
{
	None UMETA(DisplayName = "None"),
	Walking UMETA(DisplayName = "Walking"),
	TalkingRequest UMETA(DisplayName = "TalkingRequest"),
	Talking UMETA(DisplayName = "Talking")
};

UCLASS()
class GHOSTLYANTICS_API AVillageResident : public AGACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVillageResident();

protected:
	// To add mapping context
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	EVillagerState VillagerState;
};
