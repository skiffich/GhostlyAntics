// Fill out your copyright notice in the Description page of Project Settings.


#include "../InteractableItems/InteractableItemPawn.h"
#include "../Characters/GhostCharacter/GhostCharacter.h"


// Sets default values
AInteractableItemPawn::AInteractableItemPawn()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create and setup the static mesh
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = StaticMesh;

    // Create and setup the collision sphere
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(RootComponent);
    InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableItemPawn::OnOverlapBegin);
    InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableItemPawn::OnOverlapEnd);

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

    InteractionState = EInteractionState::None;
}

// Called when the game starts or when spawned
void AInteractableItemPawn::BeginPlay()
{
	Super::BeginPlay();
	
    InteractionState = EInteractionState::Ready;
}

void AInteractableItemPawn::BeginInteract(APawn* InstigatorPawn)
{
    if (InteractionState != EInteractionState::Ready)
    {
        return;
    }
    InteractionState = EInteractionState::BeginInteract;
}

void AInteractableItemPawn::StopInteract(APawn* InstigatorPawn)
{
    if (InteractionState != EInteractionState::BeginInteract)
    {
        return;
    }

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

void AInteractableItemPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AGhostCharacter* Player = Cast<AGhostCharacter>(OtherActor);
    if (Player)
    {
        Player->SetCurrentInteractable(this);
    }
}

void AInteractableItemPawn::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AGhostCharacter* Player = Cast< AGhostCharacter>(OtherActor);
    if (Player)
    {
        Player->SetCurrentInteractable(nullptr);
    }
}

