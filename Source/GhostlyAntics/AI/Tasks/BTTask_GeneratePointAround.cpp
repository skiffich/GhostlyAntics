// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GeneratePointAround.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"

UBTTask_GeneratePointAround::UBTTask_GeneratePointAround()
{
    NodeName = "Get Target Point Around Origin";

    OriginLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GeneratePointAround, OriginLocationKey));
    TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GeneratePointAround, TargetLocationKey));
}

EBTNodeResult::Type UBTTask_GeneratePointAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Getting the blackboard component and AI controller
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    AAIController* AICon = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AICon->GetPawn();

    if (!IsValid(BlackboardComp) || !IsValid(AICon) || !IsValid(ControlledPawn))
    {
        return EBTNodeResult::Failed;
    }

    // Getting the origin location from the blackboard
    FVector OriginLocation = BlackboardComp->GetValueAsVector(OriginLocationKey.SelectedKeyName);

    // Generating a random point in the specified radius
    FNavLocation RandomPoint;
    bool bFound = false;
    int Attempt = 0;
    do
    {
        if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AICon->GetWorld()))
        {
            if (NavSys->GetRandomReachablePointInRadius(OriginLocation, Radius, RandomPoint))
            {
                float DistanceToPawn = FVector::DistXY(RandomPoint.Location, ControlledPawn->GetActorLocation());
                if (DistanceToPawn > MinDistance && DistanceToPawn < MaxDistance)
                {
                    bFound = true;
                }
            }
        }
    } while (++Attempt < MaxAttemptsToFindPoint && !bFound);

    // Setting the target location on the blackboard
    if (bFound)
    {
        BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, RandomPoint.Location);
        return EBTNodeResult::Succeeded;
    }
    BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, OriginLocation);
    return EBTNodeResult::Failed;
}

