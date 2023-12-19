// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "VillagerAIController.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API AVillagerAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    AVillagerAIController();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void OnPossess(APawn* InPawn) override;

private:
    // Behavior Tree component reference
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
    UBehaviorTreeComponent* BehaviorComp;

    // Blackboard component reference
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
    UBlackboardComponent* BlackboardComp;

    // The behavior tree that contains the logic for the villager
    UPROPERTY(EditDefaultsOnly, Category = "AI", meta=(AllowPrivateAccess = true))
    UBehaviorTree* BehaviorTree;

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
};
