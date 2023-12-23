// Fill out your copyright notice in the Description page of Project Settings.


#include "../Controllers/GhostController.h"
#include "../Actors/Characters/GhostCharacter/GhostCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Actors/InteractableItems/InteractableItemPawn.h"

AGhostController::AGhostController()
{
    TraceAreaBoxSize = FVector(200.f, 100.f, 200.f);
    TraceAreaBoxAngle = 10;
}

void AGhostController::BeginPlay()
{
    Super::BeginPlay();
    StartActorCheckTimer();
}

void AGhostController::StartActorCheckTimer()
{
    // Start a repeating timer
    GetWorldTimerManager().SetTimer(ActorCheckTimerHandle, this, &AGhostController::GetActorsInScreenArea, 0.1f, true);
}

void AGhostController::GetActorsInScreenArea()
{
    // Get the player's character
    AGhostCharacter* GhostCharacter = Cast<AGhostCharacter>(GetPawn());
    if (!GhostCharacter)
    {
        return;
    }

    // Determine the center of the screen
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    FVector2D ScreenCenter = FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);

    // Convert screen coordinates to world space
    FVector WorldStartCentr, WorldDirectionCentr;
    DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldStartCentr, WorldDirectionCentr);
    FVector WorldEndCentr = WorldStartCentr + (WorldDirectionCentr * (GhostCharacter->GetCameraBoom()->TargetArmLength + TraceAreaBoxSize.X / 2.5));

    // Define the trace box
    FVector BoxExtent = TraceAreaBoxSize / 2; // Half-size of the box
    FCollisionShape Box = FCollisionShape::MakeBox(BoxExtent);

    // get box rotation in WorldDirectionCentr direction
    FQuat BoxRotation = FQuat(WorldDirectionCentr.Rotation());
    
    // Assuming 'angle' is your rotation angle in degrees
    float AngleInRadians = FMath::DegreesToRadians(TraceAreaBoxAngle);

    // get right axix of the box
    FVector BoxRightVector = FVector::CrossProduct(WorldDirectionCentr.GetSafeNormal(), FVector::UpVector);
    // Draw the right vector of the box
    //DrawDebugLine(GetWorld(), WorldEndCentr, WorldEndCentr + BoxRightVector * BoxExtent.Size(), FColor::Red, false, 2.0f, 0, 5);

    // Create a quaternion representing the horizontal rotation around the right axis
    FQuat HorizontalRotation = FQuat(BoxRightVector, AngleInRadians);

    // Combine with the existing BoxRotation
    BoxRotation = HorizontalRotation * BoxRotation;

    // Perform the box trace
    TArray<FHitResult> HitResults;
    GetWorld()->SweepMultiByChannel(HitResults, WorldEndCentr, WorldEndCentr + WorldDirectionCentr * BoxExtent.Size(), BoxRotation, ECC_Visibility, Box);

    // Draw the debug box
    DrawDebugBox(GetWorld(), WorldEndCentr, BoxExtent, BoxRotation, FColor::Green, false, 1.0f, 0, 5);
    // draw line from the player to end of the box
    //DrawDebugLine(GetWorld(), GhostCharacter->GetActorLocation(), GhostCharacter->GetActorLocation() + WorldDirectionCentr * BoxExtent.Size() * 2, FColor::Red, false, 2.0f, 0, 5);

    // Process hit results
    for (const FHitResult& Hit : HitResults.FilterByPredicate(
        [](const FHitResult& HitResult)
    {
        // Attempt to cast the hit actor to AInteractableItemPawn
        AInteractableItemPawn* ItemPawn = Cast<AInteractableItemPawn>(HitResult.GetActor());

        // Return true if the cast is successful, false otherwise
        return ItemPawn != nullptr;
    }))
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            if (AInteractableItemPawn* item = Cast<AInteractableItemPawn>(HitActor))
            {
                UE_LOG(LogTemp, Display, TEXT("AInteractableItemPawn %p %s is available"), item, *item->GetName());
            }
        }
    }
}
