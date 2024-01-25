// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include <Materials/MaterialInstance.h>

// Sets default values for this component's properties
USlotComponent::USlotComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 1.0f;

	// Load the Static Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/EditorResources/FieldNodes/_Resources/SM_FieldArrow.SM_FieldArrow'"));
	if (StaticMeshAsset.Succeeded())
	{
		//Slot->SetStaticMesh();
		SetStaticMesh(StaticMeshAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Failed"));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> AvailableMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Engine/EditorResources/FieldNodes/_Resources/MI_FieldRadiusPreview.MI_FieldRadiusPreview'"));
	if (AvailableMaterialAsset.Succeeded())
	{
		AvailableMaterial = AvailableMaterialAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> OccupiedMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Engine/EditorResources/FieldNodes/_Resources/MI_FieldVolumePreview.MI_FieldVolumePreview'"));
	if (OccupiedMaterialAsset.Succeeded())
	{
		OccupiedMaterial = OccupiedMaterialAsset.Object;
	}
	SetMaterial(0, AvailableMaterial);

	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	//Slot->SetVisibility(false, true);
	//Slot->SetHiddenInGame(true);

	State = ESlotState::Available;
}


// Called when the game starts
void USlotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (State == ESlotState::Occupied && !ClaimedActor.IsValid())
	{
		Release();
	}
}

bool USlotComponent::Claim(AActor* ClaimActor)
{
	if (IsAvailable() && IsValid(ClaimActor))
	{
		State = ESlotState::Occupied;
		this->ClaimedActor = ClaimActor;

		SetMaterial(0, OccupiedMaterial);

		return true;
	}
	return false;
}

bool USlotComponent::Release(AActor* ReleasedActor)
{
	if (State == ESlotState::Occupied && ((ClaimedActor.IsValid() && ReleasedActor == ClaimedActor) || !ClaimedActor.IsValid()))
	{
		State = ESlotState::Available;
		ClaimedActor = nullptr;

		SetMaterial(0, AvailableMaterial);

		return true;
	}
	return false;
}

bool USlotComponent::IsAvailable()
{
	return State == ESlotState::Available;
}
