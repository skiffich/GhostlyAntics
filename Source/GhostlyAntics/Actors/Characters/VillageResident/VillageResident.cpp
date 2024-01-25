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

    TalkingSlot = CreateDefaultSubobject<USlotComponent>(TEXT("Talking Slot"));
    TalkingSlot->SetRelativeLocation(FVector(.0f, 150.0f, .0f));
    TalkingSlot->SetRelativeRotation(FRotator(.0f, -90.0f, .0f));
    TalkingSlot->SetupAttachment(VillagerMesh);

	VillagerState = EVillagerState::None;
}

TArray<USlotComponent*> AVillageResident::GetVisibleSlotsInRadius()
{
	FTimespan NowTimeSpan = FTimespan::FromSeconds(FDateTime::Now().GetTimeOfDay().GetTotalSeconds());
	if ((NowTimeSpan - LastFoundSlotsTimeSpan).GetTotalSeconds() < CheckSlotsPerSeconds)
	{
		return LastFoundSlots;
	}

    // Get the current location and forward vector of the actor
    FVector ActorLocation = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();

    // Calculate the position in front of the actor
    FVector SphereCenter = ActorLocation + (ForwardVector * (SightRadius - 50.f)); // include actual actor

    // Draw a debug sphere if you're in development mode
#if WITH_EDITOR
    int32 DebugSphereSegments = 12; // Match the segments from the Blueprint
    DrawDebugSphere(
        GetWorld(),
        SphereCenter,
        SightRadius,
        DebugSphereSegments,
        FColor::Blue, // Choose the color you want for the debug sphere
        false,          // Persistent lines
        -1.f,           // How long to draw the sphere
        (uint8)'\000',  // Depth priority
        0.0f            // Thickness
    );
#endif

    // Array to hold the results
    TArray<UPrimitiveComponent*> OverlappingComponents;

    // Define the object types to query
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

    // Define a filter for the component class, if needed
    TSubclassOf<UActorComponent> ComponentFilter = USlotComponent::StaticClass();

    // Perform the sphere overlap components check
    bool bFoundComponents = UKismetSystemLibrary::SphereOverlapComponents(
        this,
        SphereCenter,
        SightRadius,
        ObjectTypes,
        ComponentFilter,
        TArray<AActor*>{this}, // ignore current actor
        OverlappingComponents
    );

    if (OverlappingComponents.Num() == 0)
    {
        return TArray<USlotComponent*>();
    }

    LastFoundSlots.Empty();

    for (UPrimitiveComponent* Component : OverlappingComponents)
    {
        if (USlotComponent* Slot = Cast<USlotComponent>(Component))
        {
            LastFoundSlots.Add(Slot);
        }
        else
        {
            return TArray<USlotComponent*>();
        }
    }

    LastFoundSlotsTimeSpan = FTimespan::FromSeconds(FDateTime::Now().GetTimeOfDay().GetTotalSeconds());

	return TArray<USlotComponent*>();
}

bool AVillageResident::IsThereVisibleTalkingSlot()
{
    TArray<USlotComponent*> VisibleSlots = GetVisibleSlotsInRadius();

    return VisibleSlots.Num() > 0;
}

// Called when the game starts or when spawned
void AVillageResident::BeginPlay()
{
	VillagerState = EVillagerState::WalkingAroundLocation;

	SpawnPoint = GetActorLocation();

	// Call the base class  
	Super::BeginPlay();

	FTimespan NowTimeSpan = FTimespan::FromSeconds(FDateTime::Now().GetTimeOfDay().GetTotalSeconds());
}

// Called every frame
void AVillageResident::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
