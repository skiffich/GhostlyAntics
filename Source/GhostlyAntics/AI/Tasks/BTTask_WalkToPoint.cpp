// Fill out your copyright notice in the Description page of Project Settings.


#include "../../AI/Tasks/BTTask_WalkToPoint.h"
#include "../VillagerAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Tasks/AITask_MoveTo.h"
#include "VisualLogger/VisualLogger.h"

// Define a console variable
static TAutoConsoleVariable<int32> CVarDrawAIDestinationPoint(
    TEXT("Villager.AI.DrawAIDestinationPoint"),
    0,
    TEXT("Draw debug AI Destination Point\n")
    TEXT("0: Do not draw debug AI Destination Point (default)\n")
    TEXT("1: Draw debug AI Destination Point"),
    ECVF_Cheat);

UBTTask_WalkToPoint::UBTTask_WalkToPoint()
{
    NodeName = TEXT("BTTask_WalkToPoint");
}

void UBTTask_WalkToPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    if (CVarDrawAIDestinationPoint.GetValueOnAnyThread() == 1 && ExecutionResult == EBTNodeResult::Type::InProgress)
    {
        DrawDebugSphere(GetWorld(), DestinationLocation, 25.0f, 12, FColor::Green, false, TickTime + TickTime * 0.1);
    }

    if (AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
        {
            if (BlackboardComp->GetValueAsBool("InviteToTalk"))
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            }
        }
    }

    SetNextTickTime(NodeMemory, TickTime);
}


EBTNodeResult::Type UBTTask_WalkToPoint::PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    bNotifyTick = true;
    bTickIntervals = true;
    SetNextTickTime(NodeMemory, 0.01);

    FBTMoveToTaskMemory* MyMemory = CastInstanceNodeMemory<FBTMoveToTaskMemory>(NodeMemory);
    AAIController* MyController = OwnerComp.GetAIOwner();

    EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;
    if (MyController)
    {
        FAIMoveRequest MoveReq;
        MoveReq.SetNavigationFilter(*FilterClass ? FilterClass : MyController->GetDefaultNavigationFilterClass());
        MoveReq.SetAllowPartialPath(bAllowPartialPath);
        MoveReq.SetAcceptanceRadius(AcceptableRadius);
        MoveReq.SetCanStrafe(bAllowStrafe);
        MoveReq.SetReachTestIncludesAgentRadius(bReachTestIncludesAgentRadius);
        MoveReq.SetReachTestIncludesGoalRadius(bReachTestIncludesGoalRadius);
        MoveReq.SetProjectGoalLocation(bProjectGoalLocation);
        MoveReq.SetUsePathfinding(bUsePathfinding);

        if (GetRandomPointInRadius(OwnerComp))
        {
            MoveReq.SetGoalLocation(DestinationLocation);
            MyMemory->PreviousGoalLocation = DestinationLocation;
        }

        if (MoveReq.IsValid())
        {
            UAITask_MoveTo* MoveTask = MyMemory->Task.Get();
            const bool bReuseExistingTask = (MoveTask != nullptr);

            MoveTask = PrepareMoveTask(OwnerComp, MoveTask, MoveReq);
            if (MoveTask)
            {
                MyMemory->bObserverCanFinishTask = false;

                if (bReuseExistingTask)
                {
                    if (MoveTask->IsActive())
                    {
                        UE_VLOG(MyController, LogBehaviorTree, Display, TEXT("\'%s\' reusing AITask %s"), *GetNodeName(), *MoveTask->GetName());
                        MoveTask->ConditionalPerformMove();
                    }
                    else
                    {
                        UE_VLOG(MyController, LogBehaviorTree, Display, TEXT("\'%s\' reusing AITask %s, but task is not active - handing over move performing to task mechanics"), *GetNodeName(), *MoveTask->GetName());
                    }
                }
                else
                {
                    MyMemory->Task = MoveTask;
                    UE_VLOG(MyController, LogBehaviorTree, Display, TEXT("\'%s\' task implementing move with task %s"), *GetNodeName(), *MoveTask->GetName());
                    MoveTask->ReadyForActivation();
                }

                MyMemory->bObserverCanFinishTask = true;
                NodeResult = (MoveTask->GetState() != EGameplayTaskState::Finished) ? EBTNodeResult::InProgress :
                    MoveTask->WasMoveSuccessful() ? EBTNodeResult::Succeeded :
                    EBTNodeResult::Failed;
            }
        }
    }

    ExecutionResult = NodeResult;
    return NodeResult;
}

bool UBTTask_WalkToPoint::GetRandomPointInRadius(const UBehaviorTreeComponent& OwnerComp)
{
    if (AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController->GetWorld()))
        {
            if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
            {
                FVector OriginLocation = BlackboardComp->GetValueAsVector("OriginLocation");
                FNavLocation RandomPoint;
                int Attempt = 0;
                do
                {
                    if (NavSys->GetRandomReachablePointInRadius(OriginLocation, WalkingRadius, RandomPoint))
                    {
                        if (APawn* ControlledPawn = AIController->GetPawn())
                        {
                            double Distance = FVector::Distance(ControlledPawn->GetActorLocation(), RandomPoint.Location);
                            if (Distance > MinDistance)
                            {
                                DestinationLocation = RandomPoint.Location;
                                return true;
                            }
                        }
                    }
                } while (++Attempt < MaxAttemptsToFindPoint);
            }
        }
    }

    return false;
}
