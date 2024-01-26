// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageResident.h"
#include "../../../AI/VillagerAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AVillageResident::AVillageResident()
{
	AIControllerClass = AVillagerAIController::StaticClass();

	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bUseRVOAvoidance = true;

	VillagerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Villager Mesh"));
	VillagerMesh->SetupAttachment(Super::GetMesh());
	VillagerMesh->SetRelativeLocation(FVector(.0f, .0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	Super::GetMesh()->SetVisibility(false);
	Super::GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

    TalkingSlot = CreateDefaultSubobject<USlotComponent>(TEXT("Talking Slot"));
    TalkingSlot->SetRelativeLocation(FVector(.0f, 150.0f, .0f));
    TalkingSlot->SetRelativeRotation(FRotator(.0f, -90.0f, .0f));
    TalkingSlot->SetupAttachment(VillagerMesh);
}