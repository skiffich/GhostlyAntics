// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/InteractInterface.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "InteractableItem.generated.h"

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
class GHOSTLYANTICS_API AInteractableItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInteractableItem();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	EInteractionState InteractionState;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* SoundToPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	EInteractActionType InteractActionType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float HardInteractionTime;

	/* UI Interaction Widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* InteractionWidgetComponent;

	/** UI Interaction Widget offset */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	float InteractionWidgetOffsetAboveMesh{ 100.0f };

	/** NavModifier */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Navigation, meta = (AllowPrivateAccess = "true"))
	class UNavModifierComponent* NavModifier;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact();
	virtual void FinishInteract();

public:
	virtual void BeginInteract() override;
	virtual void StopInteract() override;

	void ShowUITip();

	inline EInteractActionType GetInteractActionType() { return InteractActionType; }

private:
	FTimerHandle InteractionTimerHandle;
	class UIteractibleItemUI* InteractibleWidget;

	FTimerHandle UpdateUITimerHandle;
	FDateTime InteractionStartTime;

	bool isHardInteract = false;

	void UpdateUI();

};
