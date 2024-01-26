// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GACharacter/GACharacter.h"
#include "../../../AI/Slot/SlotComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "VillageResident.generated.h"



UCLASS()
class GHOSTLYANTICS_API AVillageResident : public AGACharacter
{
	GENERATED_BODY()

protected:

	// The behavior tree that contains the logic for the villager
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, meta = (category = "AI"))
	USlotComponent* TalkingSlot;

	UPROPERTY(EditAnywhere, meta = (category = "Mesh"))
	USkeletalMeshComponent* VillagerMesh;

public:
	// Sets default values for this character's properties
	AVillageResident();

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
};
