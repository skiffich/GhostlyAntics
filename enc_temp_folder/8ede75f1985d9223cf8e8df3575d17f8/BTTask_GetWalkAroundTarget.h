// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetWalkAroundTarget.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API UBTTask_GetWalkAroundTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UBTTask_GetWalkAroundTarget();

protected:
    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowedTypes = "Vector"))
    FBlackboardKeySelector WanderAroundPoint;

    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowedTypes = "Vector"))
    FBlackboardKeySelector TargetLocation;

    UPROPERTY(EditAnywhere, Category = "Config")
    int32 MaxAttemps;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    bool FindRandomReachableLocation(const class AAIController* AIController, const FVector& Origin, FVector& OutLocation);
};
