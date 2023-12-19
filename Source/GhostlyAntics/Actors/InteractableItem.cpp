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

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
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

void AInteractableItem::BeginInteract(APawn* InstigatorPawn)
{
}

void AInteractableItem::StopInteract(APawn* InstigatorPawn)
{
    // return if it is still in action
    if (StaticMesh->IsSimulatingPhysics())
    {
        return;
    }

    // Enable physics simulation
    StaticMesh->SetSimulatePhysics(true);

    // save previous location
    FVector PrevLocation = GetActorLocation();

    // Move the actor up 
    StaticMesh->AddForce(FVector(0.f, 0.f, StaticMesh->CalculateMass() * 30000));

    // Declare a local timer handle
    FTimerHandle PhysicsTimerHandle;

    // Set a timer to disable physics after 5 seconds using a lambda
    GetWorld()->GetTimerManager().SetTimer(PhysicsTimerHandle, [this, PrevLocation]()
    {
        StaticMesh->SetSimulatePhysics(false);
        SetActorLocation(PrevLocation);
    }, 5.0f, false);

    if (SoundToPlay)
    {
        AudioComponent->SetSound(SoundToPlay);
        AudioComponent->Play();

        MakeNoise(1.f, InstigatorPawn, GetActorLocation());
    }
}

