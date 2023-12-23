// Fill out your copyright notice in the Description page of Project Settings.


#include "../InteractableItems/InteractableItemPawn.h"
#include "../Characters/GhostCharacter/GhostCharacter.h"
#include "../../UI/Widgets/IteractibleItemUI.h"


// Sets default values
AInteractableItemPawn::AInteractableItemPawn()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create and setup the static mesh
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = StaticMesh;

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

    InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget Component"));
    InteractionWidgetComponent->SetupAttachment(RootComponent);
    InteractionWidgetComponent->SetDrawSize(FVector2D(200, 200)); // Set the size of the widget
    InteractionWidgetComponent->SetPivot(FVector2D(0.5f, 0.5f)); // Set the pivot for alignment
    InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // Set widget to be in screen space
    InteractionWidgetComponent->SetCullDistance(FMath::Square(1000.0f)); // appropriate culling distances to ensure the widget is not rendered when it's far away from the camera
    InteractionWidgetComponent->SetVisibility(false);

    InteractionState = EInteractionState::None;

    HardInteractionTime = 3.0f;
}

// Called when the game starts or when spawned
void AInteractableItemPawn::BeginPlay()
{
	Super::BeginPlay();

    UUserWidget* Widget = InteractionWidgetComponent->GetUserWidgetObject();
    if (Widget)
    {
        InteractibleWidget = Cast<UIteractibleItemUI>(Widget);
    }

    // Adjust these values as needed for correct positioning
    float OffsetAboveMesh = 100.0f; // Example offset

    // During initialization or after the static mesh has been configured
    FVector MeshBounds = StaticMesh->Bounds.BoxExtent;
    FVector WidgetPosition = FVector(0.0f, 0.0f, MeshBounds.Z + OffsetAboveMesh);
    InteractionWidgetComponent->SetRelativeLocation(WidgetPosition);
	
    InteractionState = EInteractionState::Ready;
}

void AInteractableItemPawn::BeginInteract()
{
    if (InteractionState != EInteractionState::Ready)
    {
        return;
    }
    InteractionState = EInteractionState::BeginInteract;

    InteractionStartTime = FDateTime::Now();
    GetWorld()->GetTimerManager().SetTimer(UpdateUITimerHandle, this, &AInteractableItemPawn::UpdateUI, HardInteractionTime / 100.0f, true);
}

void AInteractableItemPawn::StopInteract()
{
    if (InteractionState != EInteractionState::BeginInteract)
    {
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(UpdateUITimerHandle);

    if (isHardInteract)
    {
        UE_LOG(LogTemp, Display, TEXT("Hard Interaction with %s"), *GetName());
    }

    InteractibleWidget->UpdateInteractionValue(0.0f);

    Interact();
}

void AInteractableItemPawn::Interact()
{
    InteractionState = EInteractionState::InAction;
}

void AInteractableItemPawn::FinishInteract()
{
    InteractionState = EInteractionState::Ready;
}

void AInteractableItemPawn::EnableInteraction()
{
    if (!GetWorld()->GetTimerManager().IsTimerActive(InteractionTimerHandle))
    {
        InteractionWidgetComponent->SetVisibility(true);
    }
    GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle,
        [this]() 
        {
            InteractionWidgetComponent->SetVisibility(false);
        },
        0.1f, false);
}

void AInteractableItemPawn::UpdateUI()
{
    FDateTime CurrentTime = FDateTime::Now();
    FTimespan ElapsedTime = CurrentTime - InteractionStartTime;
    float ElapsedSeconds = ElapsedTime.GetTotalSeconds();

    if (ElapsedSeconds > HardInteractionTime)
    {
        isHardInteract = true;
        StopInteract();
        isHardInteract = false;
        return;
    }

    float PercentageElapsed = ElapsedSeconds / HardInteractionTime;

    InteractibleWidget->UpdateInteractionValue(PercentageElapsed);
}
