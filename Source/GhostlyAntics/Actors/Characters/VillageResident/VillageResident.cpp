// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageResident.h"
#include "../../../AI/VillagerAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Components/CapsuleComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include <functional>


// Sets default values
AVillageResident::AVillageResident()
{
	AIControllerClass = AVillagerAIController::StaticClass();

	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bUseRVOAvoidance = true;

	VillagerState = EVillagerState::None;
}

// Called when the game starts or when spawned
void AVillageResident::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	VillagerState = EVillagerState::Walking;
}

// Called every frame
void AVillageResident::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
