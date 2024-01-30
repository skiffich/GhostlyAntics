// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/SoftObjectPtr.h"
#include "GameplayTagContainer.h"
#include "SlotComponent.generated.h"

enum class ESlotState : uint8
{
	None UMETA(DisplayName = "None"),
	Available UMETA(DisplayName = "Available"),
	Occupied UMETA(DisplayName = "Occupied")
};

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API USlotComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USlotComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	bool Claim(AActor* ClaimActor);

	UFUNCTION(BlueprintCallable)
	bool Release(AActor* ReleasedActor = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsAvailable();

	// Gameplay tags for this component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer GameplayTags;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AActor* GetClaimedActor() { return ClaimedActor.Get(); }

private:
	ESlotState State;
	TSoftObjectPtr<AActor> ClaimedActor;

	UPROPERTY()
	UStaticMeshComponent* Slot;
	UMaterialInstance* AvailableMaterial;
	UMaterialInstance* OccupiedMaterial;
};
