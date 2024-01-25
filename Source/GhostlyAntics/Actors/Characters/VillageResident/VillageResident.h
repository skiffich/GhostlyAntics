// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Characters/GACharacter/GACharacter.h"
#include "../Plugins/Runtime/GameplayStateTree/Source/GameplayStateTreeModule/Public/Components/StateTreeComponent.h"
#include "Components/SplineComponent.h"
#include "../../../AI/Slot/SlotComponent.h"
#include "VillageResident.generated.h"

UENUM(BlueprintType)
enum class EVillagerState : uint8
{
	None UMETA(DisplayName = "None"),
	WalkingAroundLocation UMETA(DisplayName = "WalkingAroundLocation"),
	WalkingALongSpline UMETA(DisplayName = "WalkingALongSpline"),
	WantToTalk UMETA(DisplayName = "WantToTalk"),
	Talking UMETA(DisplayName = "Talking")
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
	FORCEINLINE void SetState(EVillagerState newState) { VillagerState = newState; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<USlotComponent*> GetVisibleSlotsInRadius();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsThereVisibleTalkingSlot();

protected:
	// To add mapping context
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleInstanceOnly)
	EVillagerState VillagerState;

	UPROPERTY(EditDefaultsOnly)
	float SightRadius = 300.f;

	UPROPERTY(VisibleInstanceOnly)
	FVector SpawnPoint;

	/* Min time in seconds which should be passed before new check */
	const float CheckSlotsPerSeconds = 3.0f;

	TArray<USlotComponent*> LastFoundSlots;
	FTimespan LastFoundSlotsTimeSpan;
};
