// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GhostController.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API AGhostController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGhostController();

	virtual void BeginPlay() override;

private:
	void StartActorCheckTimer();
	void GetActorsInScreenArea();

	FTimerHandle ActorCheckTimerHandle;

	// Size of the trace box. x - depth; y - width; z - heigh
	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector TraceAreaBoxSize;

	// Size of the trace box. x - depth; y - width; z - heigh
	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float TraceAreaBoxAngle;
};
