// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Actors/Components/SlotComponent.h"
#include "Actors/Miscellaneous/VillagerPath.h"
#include "VillagerAIController.generated.h"

UENUM(BlueprintType)
enum class EVillagerState : uint8
{
	Chilling UMETA(DisplayName = "Chilling"),
	WalkingAround UMETA(DisplayName = "Walking Around"),
	WalkingAlongPath UMETA(DisplayName = "Walking Along Path"),
	Talking UMETA(DisplayName = "Talking"),
	Interacting UMETA(DisplayName = "Interacting")
};

UENUM(BlueprintType)
enum class EStressState : uint8
{
	Calm UMETA(DisplayName = "Calm"), // Stress is 0.0 - 0.25
	Anxious UMETA(DisplayName = "Anxious"), // Stress is 0.26 - 0.5
	Frightened UMETA(DisplayName = "Frightened"), // Stress is 0.51 - 0.75
	Panic UMETA(DisplayName = "Panic") // Stress is 0.75 - 1.0
};

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API AVillagerAIController : public AAIController
{
	GENERATED_BODY()

public:
	AVillagerAIController();

private:
	// AI Perception Component reference
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EVillagerState GetState() { return VillagerState; }

	UFUNCTION(BlueprintCallable)
	void SetState(EVillagerState newState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	double GetCurrentStateDuration() const;

private:
	TArray<UPrimitiveComponent*> GetVisibleComponents() const;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<USlotComponent*> GetVisibleSlots() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<USlotComponent*> GetVisibleTalkingSlots() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsThereVisibleTalkingSlot() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const TArray<AVillagerPath*> GetVisiblePaths() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsThereVisiblePath() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EVillagerState DecideWhatToDo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EStressState GetStressState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetSightRadius() const { return SightRadius; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetPathFollowingDirection() const { return bPathFollowingDirection; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetPathFollowingDirection(bool newDirrection) { bPathFollowingDirection = newDirrection; }

private:
	EVillagerState VillagerState;

	FVector WalkAroundPoint;

	bool bPathFollowingDirection = false;

	/* Min time in seconds which should be passed before new check */
	const float MIN_CHECK_PERIOD = 1.0f;

	mutable TArray<UPrimitiveComponent*> LastFoundComponents;
	mutable FTimespan LastFoundComponentsTimeSpan;

	FTimespan StateChangedTimeSpan;

	float SightRadius;
	float Energy;
	float Intelligence;
	float Stress;
};
