// Fill out your copyright notice in the Description page of Project Settings.


#include "../Controllers/GhostController.h"
#include "../Actors/Characters/GhostCharacter/GhostCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Actors/InteractableItems/InteractableItemPawn.h"

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

    // Calculate top-left and bottom-right coordinates of the screen area
    FVector2D TopLeft = FVector2D(ScreenCenter.X - 50, ScreenCenter.Y - 100);
    FVector2D BottomRight = FVector2D(ScreenCenter.X + 50, ScreenCenter.Y + 100);

    // Convert screen coordinates to world space
    FVector WorldStart, WorldDirection;
    DeprojectScreenPositionToWorld(TopLeft.X, TopLeft.Y, WorldStart, WorldDirection);
    FVector WorldEnd = WorldStart + (WorldDirection * GhostCharacter->GetCameraBoom()->TargetArmLength);

    FVector WorldStartBR, WorldDirectionBR;
    DeprojectScreenPositionToWorld(BottomRight.X, BottomRight.Y, WorldStartBR, WorldDirectionBR);
    FVector WorldEndBR = WorldStartBR + (WorldDirectionBR * GhostCharacter->GetCameraBoom()->TargetArmLength);

    FVector WorldStartCentr, WorldDirectionCentr;
    DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldStartCentr, WorldDirectionCentr);
    FVector WorldEndCentr = WorldStartCentr + (WorldDirectionCentr * (GhostCharacter->GetCameraBoom()->TargetArmLength + 200.0f));

    // Define the trace box
    // depth, width, height
    FVector BoxExtent = FVector((WorldEnd - WorldEndBR).Size() / 2, 50, 100); // Half-size of the box
    FCollisionShape Box = FCollisionShape::MakeBox(BoxExtent);

    FQuat BoxRotation = FQuat(WorldDirectionCentr.Rotation());

    // Assuming 'angle' is your rotation angle in degrees
    float AngleInRadians = FMath::DegreesToRadians(-20);

    // Create a quaternion representing the horizontal rotation around the Z-axis
    FQuat HorizontalRotation = FQuat(BoxRotation.GetRotationAxis(), AngleInRadians);

    // Combine with the existing BoxRotation
    FQuat BoxRotation2 = FQuat(BoxRotation.Vector(), FMath::DegreesToRadians(-20));

    FVector PlayerLocation = GhostCharacter->GetActorLocation();

    // Perform the box trace
    TArray<FHitResult> HitResults;
    GetWorld()->SweepMultiByChannel(HitResults, WorldEndCentr, WorldEndCentr + WorldDirectionCentr * BoxExtent.Size(), BoxRotation, ECC_Visibility, Box);

    // Draw the debug box
    DrawDebugBox(GetWorld(), WorldEndCentr, BoxExtent, BoxRotation, FColor::Green, false, 1.0f, 0, 5);
    DrawDebugBox(GetWorld(), WorldEndCentr, BoxExtent, BoxRotation2, FColor::Blue, false, 1.0f, 0, 5);
    //DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + WorldDirectionCentr * BoxExtent.Size() * 2, FColor::Red, false, 2.0f, 0, 5);

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
