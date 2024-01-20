// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageResident.h"
#include "../../../AI/VillagerAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Components/CapsuleComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../../AI/VillagerPath.h"
#include "Components/SplineMeshComponent.h"
#include <functional>


// Sets default values
AVillageResident::AVillageResident()
{
	AIControllerClass = AVillagerAIController::StaticClass();

	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bUseRVOAvoidance = true;

	StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("State Tree"));

	VillagerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Villager Mesh"));
	VillagerMesh->SetupAttachment(Super::GetMesh());
	VillagerMesh->SetRelativeLocation(FVector(.0f, .0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	Super::GetMesh()->SetVisibility(false);
	Super::GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	VillagerState = EVillagerState::None;
}

// Called when the game starts or when spawned
void AVillageResident::BeginPlay()
{
	VillagerState = EVillagerState::WalkingAroundLocation;

	SpawnPoint = GetActorLocation();

	// Call the base class  
	Super::BeginPlay();
}

// Called every frame
void AVillageResident::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
