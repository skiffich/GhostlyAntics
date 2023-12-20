// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Actors/InteractableItems/InteractableTelekinezItem.h"
#include "Math/UnrealMathUtility.h"

AInteractableTelekinezItem::AInteractableTelekinezItem()
{
    InteractTelekinezType = EInteractTelekinezType::None;
    InteractActionType = EInteractActionType::Telekinez;
    ShouldResetAfterInteraction = false;
    PhysicsSimPeriod = 5.0f;
    PhysicsForceModifier = 3.0f;
}

void AInteractableTelekinezItem::Interact()
{
    Super::Interact();

    TelekinezInit();

    switch (InteractTelekinezType)
    {
    case EInteractTelekinezType::TelekinezThrowUp:
        TelekinezThrowUp();
        break;
    case EInteractTelekinezType::TelekinezThrowDown:
        TelekinezThrowParabolic();
        break;
    case EInteractTelekinezType::TelekinezMove:
        TelekinezMove();
        break;
    default:
        InteractionState = EInteractionState::Ready;
        break;
    }

    TelekinezFinish();
}

void AInteractableTelekinezItem::TelekinezThrowUp()
{
    // Throw mesh up 
    FVector ThrowUpForce(0.f, 0.f, StaticMesh->CalculateMass() * 10000 * PhysicsForceModifier);
    StaticMesh->AddForce(ThrowUpForce);
}

void AInteractableTelekinezItem::TelekinezThrowParabolic()
{
    // Throw mesh up 
    int x = FMath::RandRange(-3333, 3333);
    FVector ThrowParabolicForce(
        StaticMesh->CalculateMass() * x * PhysicsForceModifier, 
        StaticMesh->CalculateMass() * ((6666 - FMath::Abs(x)) * (FMath::FRand() > 0.5 ? 1 : -1)) * PhysicsForceModifier,
        StaticMesh->CalculateMass() * 3333 * PhysicsForceModifier);
    StaticMesh->AddForce(ThrowParabolicForce);
}

void AInteractableTelekinezItem::TelekinezMove()
{
    // Throw mesh up 
    int x = FMath::RandRange(-5000, 5000);
    FVector MoveForce(
        StaticMesh->CalculateMass() * x * PhysicsForceModifier,
        StaticMesh->CalculateMass() * ((10000 - FMath::Abs(x)) * (FMath::FRand() > 0.5 ? 1 : -1)) * PhysicsForceModifier, 0.f);
    StaticMesh->AddForce(MoveForce);
}

void AInteractableTelekinezItem::TelekinezInit()
{
    // Enable physics simulation
    StaticMesh->SetSimulatePhysics(true);

    // save previous state
    PrevLocation = GetActorLocation();
    PrevRotation = GetActorRotation();
}

void AInteractableTelekinezItem::TelekinezFinish()
{
    // Declare a local timer handle
    FTimerHandle PhysicsTimerHandle;

    // Set a timer to disable physics after 5 seconds using a lambda
    GetWorld()->GetTimerManager().SetTimer(PhysicsTimerHandle, [this]()
    {
        this->StaticMesh->SetSimulatePhysics(false);
        if (this->ShouldResetAfterInteraction)
        {
            SetActorLocation(this->PrevLocation);
            SetActorRotation(this->PrevRotation);
        }
        this->InteractionState = EInteractionState::Ready;
    }, PhysicsSimPeriod, false);

    if (SoundToPlay)
    {
        AudioComponent->SetSound(SoundToPlay);
        AudioComponent->Play();
    }

    MakeNoise(1.f, GetInstigator(), GetActorLocation());
}
