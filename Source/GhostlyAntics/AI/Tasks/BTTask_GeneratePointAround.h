// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GeneratePointAround.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API UBTTask_GeneratePointAround : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTTask_GeneratePointAround();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // Blackboard keys
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector OriginLocationKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector TargetLocationKey;

    // Parameters
    UPROPERTY(EditAnywhere, Category = "Search")
    float Radius = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Search")
    float MinDistance = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Search")
    float MaxDistance = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Search")
    int MaxAttemptsToFindPoint{ 5 };
};
