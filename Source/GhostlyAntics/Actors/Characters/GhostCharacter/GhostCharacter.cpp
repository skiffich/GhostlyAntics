// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostCharacter.h"
#include "../../../Controllers/GhostController.h"
#include "../../InteractableItems/InteractableItemPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "NavigationSystem.h"
#include <NavModifierComponent.h>

// Sets default values
AGhostCharacter::AGhostCharacter()
{
		// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));
    NavModifier->RegisterComponent();
    NavModifier->FailsafeExtent = FVector(50.0f, 50.0f, 100.0f);

	OverlappedInteractableItem = nullptr;
}

// Called when the game starts or when spawned
void AGhostCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void AGhostCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGhostCharacter::SetCurrentInteractable(IInteractInterface* NewInteractable)
{
	OverlappedInteractableItem = NewInteractable;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGhostCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AGhostCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AGhostCharacter::StopInteract);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGhostCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGhostCharacter::Look);

	}
}

void AGhostCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGhostCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGhostCharacter::Interact(const FInputActionValue& Value)
{
	AGhostController* GhostController = Cast<AGhostController>(GetController());
	if (GhostController)
	{
		TArray<APawn*> InteractableItems = GhostController->GetInteractibleActors();
		if (InteractableItems.Num() > 0)
		{
			if (AInteractableItemPawn* InteractableItem = Cast<AInteractableItemPawn>(InteractableItems[0]))
			{
				InteractableItem->BeginInteract();
			}
		}
	}
}

void AGhostCharacter::StopInteract(const FInputActionValue& Value)
{
	AGhostController* GhostController = Cast<AGhostController>(GetController());
	if (GhostController)
	{
		TArray<APawn*> InteractableItems = GhostController->GetInteractibleActors();
		if (InteractableItems.Num() > 0)
		{
			if (AInteractableItemPawn* InteractableItem = Cast<AInteractableItemPawn>(InteractableItems[0]))
			{
				InteractableItem->StopInteract();
			}
		}
	}
}


