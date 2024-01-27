// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/GhostController.h"
#include "Actors/Characters/GhostCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Actors/InteractableItems/InteractableItem.h"

// Define a console variable
static TAutoConsoleVariable<int32> CVarDrawDebugLinesToHitResults(
    TEXT("Ghost.DrawDebugLinesToHitResults"),
    0,
    TEXT("Draw debug lines to HitResults for GhostController.\n")
    TEXT("0: Do not draw debug lines (default)\n")
    TEXT("1: Draw debug lines"),
    ECVF_Cheat);

static TAutoConsoleVariable<int32> CVarDrawTraceBox(
    TEXT("Ghost.DrawDebugTraceBox"),
    0,
    TEXT("Draw TraceBox.\n")
    TEXT("0: Do not draw debug TraceBox (default)\n")
    TEXT("1: Draw debug TraceBox"),
    ECVF_Cheat);

static TAutoConsoleVariable<int32> CVarDrawDebugLines(
    TEXT("Ghost.DrawDebugLines"),
    0,
    TEXT("Draw BoxRightVector and BoxLine.\n")
    TEXT("0: Do not draw debug lines (default)\n")
    TEXT("1: Draw debug lines"),
    ECVF_Cheat);

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

TArray<AActor*> AGhostController::GetInteractibleActors()
{
    return InteractionItems;
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
        UE_LOG(LogTemp, Warning, TEXT("AGhostController::GetActorsInScreenArea Could not get GhostCharacter from Pawn %p"), GetPawn());
        return;
    }

    // Determine the center of the screen
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    FVector2D ScreenCenter = FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);

    // Convert screen coordinates to world space
    FVector WorldStartCentr, WorldDirectionCentr;
    DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldStartCentr, WorldDirectionCentr);
    FVector WorldEndCentr = WorldStartCentr + (WorldDirectionCentr * (GhostCharacter->GetCameraBoom()->TargetArmLength + TraceAreaBoxSize.X / 2.0));
    WorldEndCentr.Z = GhostCharacter->GetActorLocation().Z;

    // Define the trace box
    FVector BoxHalfExtent = TraceAreaBoxSize / 2; // Half-size of the box
    FCollisionShape Box = FCollisionShape::MakeBox(BoxHalfExtent);

    // get box rotation in WorldDirectionCentr direction
    FQuat BoxRotation = FQuat(WorldDirectionCentr.Rotation());

    // Assuming 'angle' is your rotation angle in degrees
    float AngleInRadians = FMath::DegreesToRadians(TraceAreaBoxAngle);

    // get right axix of the box
    FVector BoxRightVector = FVector::CrossProduct(WorldDirectionCentr.GetSafeNormal(), FVector::UpVector);
    // Draw the right vector of the box
    if (CVarDrawDebugLines.GetValueOnAnyThread() == 1)
    {
        DrawDebugLine(GetWorld(), WorldEndCentr, WorldEndCentr + BoxRightVector * BoxHalfExtent.Size(), FColor::Red, false, 0.1f, 0, 2.0);
    }

    // Create a quaternion representing the horizontal rotation around the right axis
    FQuat HorizontalRotation = FQuat(BoxRightVector, AngleInRadians);

    // Combine with the existing BoxRotation
    BoxRotation = HorizontalRotation * BoxRotation;

    // Perform the box trace
    TArray<FHitResult> HitResults;
    GetWorld()->SweepMultiByChannel(HitResults, WorldEndCentr, WorldEndCentr, BoxRotation, ECC_Pawn, Box);

    // Draw the debug box
    if (CVarDrawTraceBox.GetValueOnAnyThread() == 1)
    {
        DrawDebugBox(GetWorld(), WorldEndCentr, BoxHalfExtent, BoxRotation, FColor::Green, false, 0.1f, 0, 3.0);
    }
    // draw the line from the box centr to end of the box
    if (CVarDrawDebugLines.GetValueOnAnyThread() == 1)
    {
        DrawDebugLine(GetWorld(), WorldEndCentr, WorldEndCentr + BoxRotation.Vector() * BoxHalfExtent.Size(), FColor::Red, false, 2.0f, 0, 1);
    }

    // filter only thouse HitResults which could be casted to AInteractableItem
    HitResults = HitResults.FilterByPredicate(
        [](const FHitResult& HitResult)
    {
        // Attempt to cast the hit actor to AInteractableItem
        AInteractableItem* ItemPawn = Cast<AInteractableItem>(HitResult.GetActor());
        return ItemPawn != nullptr;
    });

    // Sort HitResults depending on distance to Hit Impact Point
    HitResults.Sort(
        [WorldEndCentr](const FHitResult& A, const FHitResult& B)
    {
        // Distance from WorldEndCentr to the Hit Impact Point
        float DistA = (A.ImpactPoint - WorldEndCentr).SizeSquared();
        float DistB = (B.ImpactPoint - WorldEndCentr).SizeSquared();

        return DistA < DistB;
    });

    // Process hit results

    if (HitResults.Num() > 0)
    {
        if (AInteractableItem* ItemPawn = Cast<AInteractableItem>(HitResults[0].GetActor()))
        {
            ItemPawn->ShowUITip();
        }
    }

    InteractionItems.Empty();
    for (const FHitResult& Hit : HitResults)
    {
        InteractionItems.Add(Cast<AInteractableItem>(Hit.GetActor()));
    }

    // Draw lines to HitResults depending on distance to Hit Impact Point
    if (CVarDrawDebugLinesToHitResults.GetValueOnGameThread() == 1)
    {
        int32 NumHits = HitResults.Num();
        for (int32 i = 0; i < NumHits; ++i)
        {
            // Calculate color of debug line depending on the distance from WorldEndCentr
            float ColorRatio = NumHits > 1 ? i / static_cast<float>(NumHits - 1) : 0.0;
            FColor LineColor = FColor::MakeRedToGreenColorFromScalar(ColorRatio); // Red is closer; Green is farther
            UE_LOG(LogTemp, Display, TEXT("%s"), *HitResults[i].GetActor()->GetName());
            DrawDebugLine(GetWorld(), GhostCharacter->GetActorLocation(), HitResults[i].GetActor()->GetActorLocation(), LineColor, false, 0.1f, 0, 3);
        }
    }
}
