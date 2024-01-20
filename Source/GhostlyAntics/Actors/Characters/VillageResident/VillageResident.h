// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Characters/GACharacter/GACharacter.h"
#include "../Plugins/Runtime/GameplayStateTree/Source/GameplayStateTreeModule/Public/Components/StateTreeComponent.h"
#include "Components/SplineComponent.h"
#include "VillageResident.generated.h"

UENUM(BlueprintType)
enum class EVillagerState : uint8
{
	None UMETA(DisplayName = "None"),
	WalkingAroundLocation UMETA(DisplayName = "WalkingAroundLocation"),
	WalkingALongSpline UMETA(DisplayName = "WalkingALongSpline"),
	TalkingRequest UMETA(DisplayName = "TalkingRequest"),
	Talking UMETA(DisplayName = "Talking")
};

UCLASS()
class GHOSTLYANTICS_API AVillageResident : public AGACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (category = "AI"))
	UStateTreeComponent* StateTree;

	UPROPERTY(EditAnywhere, meta = (category = "Mesh"))
	USkeletalMeshComponent* VillagerMesh;

public:
	// Sets default values for this character's properties
	AVillageResident();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EVillagerState GetState() { return VillagerState; }

protected:
	// To add mapping context
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleInstanceOnly)
	EVillagerState VillagerState;

	FVector SpawnPoint;
};
