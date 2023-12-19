// Fill out your copyright notice in the Description page of Project Settings.


#include "../../AI/Tasks/VillagerBTT_GoRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UVillagerBTT_GoRandomLocation::UVillagerBTT_GoRandomLocation()
{
	NodeName = TEXT("VillagerBTT_GoRandomLocation");

    // accept only vectors
    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UVillagerBTT_GoRandomLocation, BlackboardKey));
}

void UVillagerBTT_GoRandomLocation::OnGameplayTaskActivated(UGameplayTask& Task)
{
}

FString UVillagerBTT_GoRandomLocation::GetStaticDescription() const
{
    return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

EBTNodeResult::Type UVillagerBTT_GoRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* ModeMemmory)
{
    if (AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController->GetWorld()))
        {
            if (APawn* AIPawn = AIController->GetPawn())
            {
                FNavLocation RandomPoint;
                if (NavSys->GetRandomPointInNavigableRadius(AIPawn->GetActorLocation(), SearchRadius, RandomPoint))
                {
                    AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, RandomPoint.Location);
                    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }
    return EBTNodeResult::Failed;
}
