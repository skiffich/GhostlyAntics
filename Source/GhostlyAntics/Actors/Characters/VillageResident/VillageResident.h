// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Characters/GACharacter/GACharacter.h"
#include "../Plugins/Runtime/GameplayStateTree/Source/GameplayStateTreeModule/Public/Components/StateTreeComponent.h"
#include "../../../AI/VillagerPath.h"
#include "../../../AI/Slot/SlotComponent.h"
#include "VillageResident.generated.h"

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
enum class EVillagerDesiredState : uint8
{
	Chill UMETA(DisplayName = "Chill"),
	WalkAround UMETA(DisplayName = "Walk Around"),
	WalkAlongPath UMETA(DisplayName = "Walk Along Path"),
	Talk UMETA(DisplayName = "Talk"),
	Interract UMETA(DisplayName = "Interract"),
};

UENUM(BlueprintType)
enum class EStressState : uint8
{
	Calm UMETA(DisplayName = "Calm"), // Stress is 0.0 - 0.25
	Anxious UMETA(DisplayName = "Anxious"), // Stress is 0.26 - 0.5
	Frightened UMETA(DisplayName = "Frightened"), // Stress is 0.51 - 0.75
	Panic UMETA(DisplayName = "Panic") // Stress is 0.75 - 1.0
};

UCLASS()
class GHOSTLYANTICS_API AVillageResident : public AGACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (category = "AI"))
	UStateTreeComponent* StateTree;

	UPROPERTY(EditAnywhere, meta = (category = "AI"))
	USlotComponent* TalkingSlot;

	UPROPERTY(EditAnywhere, meta = (category = "Mesh"))
	USkeletalMeshComponent* VillagerMesh;

public:
	// Sets default values for this character's properties
	AVillageResident();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EVillagerState GetState() { return VillagerState; }

	UFUNCTION(BlueprintCallable)
	void SetState(EVillagerState newState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	double GetCurrentStateDuration();

private:
	TArray<UPrimitiveComponent*> GetVisibleComponents();

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<USlotComponent*> GetVisibleSlots();

	// Talking

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<USlotComponent*> GetVisibleTalkingSlots();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsThereVisibleTalkingSlot();

	// Paths
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AVillagerPath*> GetVisiblePaths();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsThereVisiblePath();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EVillagerDesiredState DecideWhatToDo();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EStressState GetStressState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetSightRadius() const { return SightRadius; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetPathFollowingDirection() const { return bPathFollowingDirection; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetPathFollowingDirection(bool newDirrection) { bPathFollowingDirection = newDirrection; }

protected:
	// To add mapping context
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleInstanceOnly)
	EVillagerState VillagerState;

	UPROPERTY(VisibleInstanceOnly)
	FVector WalkAroundPoint;

	bool bPathFollowingDirection = false;

	/* Min time in seconds which should be passed before new check */
	const float CheckPeriod = 3.0f;

	TArray<UPrimitiveComponent*> LastFoundComponents;
	FTimespan LastFoundComponentsTimeSpan;

	FTimespan StateChangedTimeSpan;

	float SightRadius;
	float Energy;
	float Intelligence;
	float Stress;
};
