// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/VillagerAIController.h"

AVillagerAIController::AVillagerAIController()
{
    // Initialize the BehaviorTreeComponent, BlackboardComponent and the corresponding key
    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AVillagerAIController::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(BehaviorTree))
    {
        RunBehaviorTree(BehaviorTree);
        BehaviorComp->StartTree(*BehaviorTree);
    }
}

void AVillagerAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Initialize the blackboard and start the behavior tree when the AI possesses a pawn
    if (IsValid(BlackboardComp) && IsValid(BehaviorTree))
    {
        BlackboardComp->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));

        if (IsValid(BehaviorComp))
        {
            BehaviorComp->StartTree(*BehaviorTree);
        }
    }
}