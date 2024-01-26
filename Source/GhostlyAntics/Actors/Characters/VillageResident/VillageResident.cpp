// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageResident.h"
#include "../../../AI/VillagerAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Components/CapsuleComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
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

    SightRadius = 300.f;
    Energy = FMath::RandRange(0.5, 1.0);
    Intelligence = FMath::RandRange(0.5, 1.0);
    Stress = FMath::RandRange(0.0, 0.2);

	VillagerState = EVillagerState::Chilling;
}

void AVillageResident::SetState(EVillagerState newState)
{
    VillagerState = newState;
    StateChangedTimeSpan = FTimespan::FromSeconds(FDateTime::Now().GetTimeOfDay().GetTotalSeconds());
}

double AVillageResident::GetCurrentStateDuration()
{
    FTimespan NowTimeSpan = FTimespan::FromSeconds(FDateTime::Now().GetTimeOfDay().GetTotalSeconds());
    return (NowTimeSpan - StateChangedTimeSpan).GetTotalSeconds();
}

TArray<UPrimitiveComponent*> AVillageResident::GetVisibleComponents()
{
    FTimespan NowTimeSpan = FTimespan::FromSeconds(FDateTime::Now().GetTimeOfDay().GetTotalSeconds());
    if ((NowTimeSpan - LastFoundComponentsTimeSpan).GetTotalSeconds() < CheckPeriod)
    {
        return LastFoundComponents;
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

    // Perform the sphere overlap components check
    bool bFoundComponents = UKismetSystemLibrary::SphereOverlapComponents(
        this,
        SphereCenter,
        SightRadius,
        ObjectTypes,
        nullptr,
        TArray<AActor*>{this}, // ignore current actor
        OverlappingComponents
    );

    LastFoundComponents = OverlappingComponents;

    return LastFoundComponents;
}

TArray<USlotComponent*> AVillageResident::GetVisibleSlots()
{
    TArray<USlotComponent*> Result;

    TArray<UPrimitiveComponent*> OverlappingComponents = GetVisibleComponents();

    for (UPrimitiveComponent* Component : OverlappingComponents)
    {
        if (USlotComponent* Slot = Cast<USlotComponent>(Component))
        {
            Result.Add(Slot);
        }
    }

	return Result;
}

TArray<USlotComponent*> AVillageResident::GetVisibleTalkingSlots()
{
    TArray<USlotComponent*> VisibleSlots = GetVisibleSlots();

    for (USlotComponent* Slot : VisibleSlots)
    {
        const FGameplayTag TalkingSlotTag = FGameplayTag::RequestGameplayTag(FName("Slots.Villager.TalkingSlot"));
        if (!Slot->GameplayTags.HasTag(TalkingSlotTag))
        {
            VisibleSlots.Remove(Slot);
        }
    }

    return VisibleSlots;
}

bool AVillageResident::IsThereVisibleTalkingSlot()
{
       return GetVisibleTalkingSlots().Num() > 0;
}

TArray<AVillagerPath*> AVillageResident::GetVisiblePaths()
{
    TArray<AVillagerPath*> Result;

    TArray<UPrimitiveComponent*> OverlappingComponents = GetVisibleComponents();

    for (UPrimitiveComponent* Component : OverlappingComponents)
    {
        if (USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Component))
        {
            if (AVillagerPath* Path = Cast<AVillagerPath>(SplineMesh->GetOwner()))
            {
                Result.Add(Path);
            }
        }
    }

    return Result;
}

bool AVillageResident::IsThereVisiblePath()
{
    return GetVisiblePaths().Num() > 0;
}

EVillagerDesiredState AVillageResident::DecideWhatToDo() 
{
    switch (GetStressState())
    {
    case EStressState::Calm:
        if (UKismetMathLibrary::RandomBoolWithWeight(1.5 * exp(1 * -Energy)))
        {
            return EVillagerDesiredState::Chill;
        }
        if (Energy > 0.75f && IsThereVisiblePath())
        {
            EVillagerDesiredState::WalkAlongPath;
        }
        if (Energy > 0.3f && IsThereVisibleTalkingSlot())
        {
            EVillagerDesiredState::Talk;
        }
           return EVillagerDesiredState::WalkAround;
        break;
    default:
        return EVillagerDesiredState::WalkAround;
    }
}

EStressState AVillageResident::GetStressState() const 
{
    if (Stress > 0.75f)
    {
        return EStressState::Panic;
    }
    if (Stress > 0.5f)
    {
        return EStressState::Frightened;
    }
    if (Stress > 0.25f)
    {
        return EStressState::Anxious;
    }
    return EStressState::Calm;
}

// Called when the game starts or when spawned
void AVillageResident::BeginPlay()
{
	WalkAroundPoint = GetActorLocation();

	// Call the base class  
	Super::BeginPlay();
}

// Called every frame
void AVillageResident::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
