// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetWalkAroundTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "../VillagerAIController.h"

UBTTask_GetWalkAroundTarget::UBTTask_GetWalkAroundTarget()
{
	NodeName = "Get Walk Around Target";

    MaxAttemps = 5;
}

EBTNodeResult::Type UBTTask_GetWalkAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AVillagerAIController* AIController = Cast<AVillagerAIController>(OwnerComp.GetAIOwner());
    if (!IsValid(AIController))
    {
        return EBTNodeResult::Failed;
    }

    AIController->SetState(EVillagerState::WalkingAround);

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    FVector VWanderAroundPoint = BlackboardComp->GetValueAsVector(WanderAroundPoint.SelectedKeyName);

    FVector NewLocation;
    if (FindRandomReachableLocation(AIController, VWanderAroundPoint, NewLocation))
    {
        BlackboardComp->SetValueAsVector(TargetLocation.SelectedKeyName, NewLocation);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

bool UBTTask_GetWalkAroundTarget::FindRandomReachableLocation(const AVillagerAIController* AIController, const FVector& Origin, FVector& OutLocation)
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys)
    {
        return false;
    }
    float SearchRadius = 300;
    if (const AVillagerAIController* VillagerAI = Cast<AVillagerAIController>(AIController))
    {
        SearchRadius = VillagerAI->GetSightRadius();
    }

    // Get forward direction
    FVector ForwardDirection = AIController->GetPawn()->GetActorForwardVector();

    // Calculate the search origin in front of the pawn
    FVector SearchOrigin = Origin + (ForwardDirection * SearchRadius);

    int AttemptsLeft = MaxAttemps;


    for (int attempt = 0; attempt < MaxAttemps; attempt++)
    {
        FNavLocation RandomPoint;
        if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, RandomPoint) 
            && FVector::Dist2D(RandomPoint.Location, AIController->GetPawn()->GetActorLocation()) > SearchRadius / 2)
        {
            OutLocation = RandomPoint.Location;
            return true;
        }
    }

    return false;
}
