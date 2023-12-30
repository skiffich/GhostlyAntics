// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/VillagerAIController.h"
#include "../Actors/Characters/VillageResident/VillageResident.h"
#include "../Actors/InteractableItems/InteractableItemPawn.h"

AVillagerAIController::AVillagerAIController()
{
    // Initialize the BehaviorTreeComponent, BlackboardComponent and the corresponding key
    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

    // Initialize the perception component
    AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

    // Initialize the sight configuration
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 900.0f;
    SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.0f;
    SightConfig->PeripheralVisionAngleDegrees = 60.0f;
    SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    // Initialize the hearing configuration
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = 500.0f;
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

    // Add the senses to the perception component
    AIPerceptionComp->ConfigureSense(*SightConfig);
    AIPerceptionComp->ConfigureSense(*HearingConfig);
    AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AVillagerAIController::OnPerceptionUpdated);

    SetVillagerAIState(EVillagerAIState::None);
}

void AVillagerAIController::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(AIPerceptionComp)) 
    {
        // Set the dominant sense
        AIPerceptionComp->SetDominantSense(*SightConfig->GetSenseImplementation());

        // Register the perception component
        SetPerceptionComponent(*AIPerceptionComp);
    }
}

void AVillagerAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Initialize the blackboard and start the behavior tree when the AI possesses a pawn
    if (IsValid(BlackboardComp) && IsValid(BehaviorTree) && IsValid(BehaviorComp))
    {
        BlackboardComp->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));

        if (IsValid(InPawn))
        {
            BlackboardComp->SetValueAsVector("OriginLocation", InPawn->GetActorLocation());
        }
        else
        {
            BlackboardComp->SetValueAsVector("OriginLocation", FVector(.0f, .0f, .0f));
        }

        BehaviorComp->StartTree(*BehaviorTree);

        SetVillagerAIState(EVillagerAIState::Walking);
    }
}

void AVillagerAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (AVillageResident* Villager = Cast<AVillageResident>(Actor))
    {
        if (Stimulus.WasSuccessfullySensed() && Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
        {
            if (AVillageResident* CurrentPawn = Cast<AVillageResident>(GetPawn()))
            {
                if (CurrentPawn->DoesWantToTalk())
                {
                    CurrentPawn->RequestConversation(CurrentPawn, Villager);
                }
            }
        }
    }
}

void AVillagerAIController::BeginTalkingWith(APawn* PawnToTalkWith)
{
    if (IsValid(BlackboardComp))
    {
        BlackboardComp->SetValueAsVector("TargetLocation", PawnToTalkWith->GetActorLocation());
        SetVillagerAIState(EVillagerAIState::Talking);
    }
}

void AVillagerAIController::SetVillagerAIState(EVillagerAIState newState)
{
    VillagerAIState = newState;
    if (IsValid(BlackboardComp))
    {
        BlackboardComp->SetValueAsEnum("VillagerAIState", static_cast<uint8>(newState));
    }
}
