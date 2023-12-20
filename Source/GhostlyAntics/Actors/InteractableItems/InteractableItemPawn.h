// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Components/SphereComponent.h>
#include "../InteractableItems/InteractInterface.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "InteractableItemPawn.generated.h"

UENUM(BlueprintType)
enum class EInteractActionType : uint8
{
	None,
	Telekinez
};

UENUM(BlueprintType)
enum class EInteractionState : uint8
{
	None UMETA(DisplayName = "None"),
	Ready UMETA(DisplayName = "Ready"),
	BeginInteract UMETA(DisplayName = "BeginInteract"),
	InAction UMETA(DisplayName = "InAction")
};

UCLASS()
class GHOSTLYANTICS_API AInteractableItemPawn : public APawn, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInteractableItemPawn();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* InteractionSphere;

	EInteractionState InteractionState;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* SoundToPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	EInteractActionType InteractActionType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact();
	virtual void FinishInteract();

public:	
	virtual void BeginInteract(APawn* InstigatorPawn) override;
	virtual void StopInteract(APawn* InstigatorPawn) override;

	inline EInteractActionType GetInteractActionType() { return InteractActionType; }

private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void TelekinezThrowUp();
};
