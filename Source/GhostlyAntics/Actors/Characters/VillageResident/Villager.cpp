// Fill out your copyright notice in the Description page of Project Settings.


#include "Villager.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AVillager::AVillager()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MedievalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MedievalSkeletalMesh"));
	MedievalMesh->SetupAttachment(GetMesh());
	MedievalMesh->SetRelativeLocation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	GetMesh()->SetVisibility(false);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

// Called when the game starts or when spawned
void AVillager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVillager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

