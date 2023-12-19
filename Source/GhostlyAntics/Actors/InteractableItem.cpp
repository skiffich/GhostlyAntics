// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/InteractableItem.h"
#include "Characters/GhostCharacter/GhostCharacter.h"

void AInteractableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AGhostCharacter* Player = Cast< AGhostCharacter>(OtherActor);
    if (Player)
    {
        Player->SetCurrentInteractable(this);
    }
}

void AInteractableItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AGhostCharacter* Player = Cast< AGhostCharacter>(OtherActor);
    if (Player)
    {
        Player->SetCurrentInteractable(nullptr);
    }
}

// Sets default values
AInteractableItem::AInteractableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create and setup the static mesh
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = StaticMesh;

    // Create and setup the collision sphere
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(RootComponent);
    InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableItem::OnOverlapBegin);
    InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableItem::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableItem::BeginInteract()
{
}

void AInteractableItem::StopInteract()
{
    UE_LOG(LogTemp, Warning, TEXT("AInteractableItem::StopInteract"));
    // Move the actor up by a certain amount
    FVector NewLocation = GetActorLocation() + FVector(0, 0, 100); // Adjust the Z-axis offset as needed
    SetActorLocation(NewLocation);

    // Enable physics simulation
    StaticMesh->SetSimulatePhysics(true);

    // Declare a local timer handle
    FTimerHandle PhysicsTimerHandle;

    // Set a timer to disable physics after 5 seconds using a lambda
    GetWorld()->GetTimerManager().SetTimer(PhysicsTimerHandle, [this]()
    {
        StaticMesh->SetSimulatePhysics(false);
    }, 5.0f, false);
}

