// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/VillagerAIController.h"
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
    SightConfig->SightRadius = 700.0f;
    SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f;
    SightConfig->PeripheralVisionAngleDegrees = 70.0f;
    SightConfig->SetMaxAge(5.0f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
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
}

void AVillagerAIController::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(BehaviorTree))
    {
        RunBehaviorTree(BehaviorTree);
        BehaviorComp->StartTree(*BehaviorTree);
    }

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
    if (IsValid(BlackboardComp) && IsValid(BehaviorTree))
    {
        BlackboardComp->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));

        if (IsValid(BehaviorComp))
        {
            BehaviorComp->StartTree(*BehaviorTree);
        }
    }
}

void AVillagerAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Stimulus.WasSuccessfullySensed())
    {
        if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Actor %s was seen"), *Actor->GetName());

            if (BlackboardComp)
            {
                FVector Location = Stimulus.StimulusLocation;
                BlackboardComp->SetValueAsVector("StimulusLocation", Location);
            }
        }
        else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Actor %s was heard"), *Actor->GetName());
        }

        if (AInteractableItemPawn* item = Cast<AInteractableItemPawn>(Actor))
        {
            UE_LOG(LogTemp, Warning, TEXT("InteractableItem %s was detected"), *Actor->GetName());

        }
    }
}
