// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "VillagerAIController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EVillagerAIState : uint8
{
    None UMETA(DisplayName = "None"),
    Walking UMETA(DisplayName = "Walking"),
    Talking UMETA(DisplayName = "Talking")
};


UCLASS()
class GHOSTLYANTICS_API AVillagerAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    AVillagerAIController();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    // AI Perception Component reference
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
    UAIPerceptionComponent* AIPerceptionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
    UAISenseConfig_Sight* SightConfig;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
    UAISenseConfig_Hearing* HearingConfig;

protected:

    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
    void SetVillagerAIState(EVillagerAIState newState);

private:
    EVillagerAIState VillagerAIState;
};
