// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/VillagerAIController.h"
#include "Actors/Characters/VillageResident.h"
#include "Actors/InteractableItems/InteractableItem.h"
#include "Actors/Miscellaneous/VillagerPath.h"
#include "Actors/Components/SlotComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineMeshComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AVillagerAIController::AVillagerAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
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


    SightRadius = 600.f;
    Energy = FMath::RandRange(0.5, 1.0);
    Intelligence = FMath::RandRange(0.5, 1.0);
    Stress = FMath::RandRange(0.0, 0.2);

    VillagerState = EVillagerState::Chilling;
}

void AVillagerAIController::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(AIPerceptionComp) && IsValid(SightConfig))
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
    if (AVillageResident* const Villager = Cast<AVillageResident>(InPawn))
    {
        if (UBehaviorTree* const Tree = Villager->GetBehaviorTree())
        {
            UBlackboardComponent* BB;
            UseBlackboard(Tree->BlackboardAsset, BB);
            Blackboard = BB;

            Blackboard->SetValueAsVector("WalkAroundPoint", Villager->GetActorLocation());
            Blackboard->SetValueAsFloat("ChillFor", 5.0f);
            Blackboard->SetValueAsObject("Interlocutor", this);

            RunBehaviorTree(Tree);
        }
    }
}

void AVillagerAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    switch (VillagerState)
    {
    case EVillagerState::Chilling:
        Energy += DeltaTime * 0.01;
        Energy = FMath::Min(Energy, 1.0f);
        break;
    case EVillagerState::WalkingAround:
        Energy -= DeltaTime * 0.005;
        Energy = FMath::Max(Energy, 0.0f);
        break;
    case EVillagerState::WalkingAlongPath:
        Energy -= DeltaTime * 0.005;
        Energy = FMath::Max(Energy, 0.0f);
        break;
    case EVillagerState::Talking:
        break;
    case EVillagerState::Interacting:
        break;
    default:
        break;
    }
}

void AVillagerAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (AVillageResident* Villager = Cast<AVillageResident>(Actor))
    {
        if (Stimulus.WasSuccessfullySensed() && Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
        {
        }
    }
}


void AVillagerAIController::SetState(EVillagerState newState)
{
    if (VillagerState != newState)
    {
        VillagerState = newState;
        StateChangedTimeSpan = FTimespan::FromSeconds(FDateTime::Now().GetTimeOfDay().GetTotalSeconds());
    }
}

double AVillagerAIController::GetCurrentStateDuration() const
{
    FTimespan NowTimeSpan = FTimespan::FromSeconds(FDateTime::Now().GetTimeOfDay().GetTotalSeconds());
    return (NowTimeSpan - StateChangedTimeSpan).GetTotalSeconds();
}

TArray<UPrimitiveComponent*> AVillagerAIController::GetVisibleComponents() const
{
    FTimespan NowTimeSpan = FTimespan::FromSeconds(FDateTime::Now().GetTimeOfDay().GetTotalSeconds());
    if ((NowTimeSpan - LastFoundComponentsTimeSpan).GetTotalSeconds() < MIN_CHECK_PERIOD)
    {
        return LastFoundComponents;
    }

    // Get the current location and forward vector of the actor
    FVector ActorLocation = GetPawn()->GetActorLocation();
    FVector ForwardVector = GetPawn()->GetActorForwardVector();

    // Calculate the position in front of the actor
    FVector SphereCenter = ActorLocation + (ForwardVector * (SightRadius - 50.f)); // include actual actor

    // Draw a debug sphere if you're in development mode
#if WITH_EDITOR
    int32 DebugSphereSegments = 12; // Match the segments from the Blueprint
    DrawDebugSphere(
        GetWorld(),
        SphereCenter,
        SightRadius,
        DebugSphereSegments,
        FColor::Blue, // Choose the color you want for the debug sphere
        false,          // Persistent lines
            0.5f,           // How long to draw the sphere
            (uint8)'\000',  // Depth priority
            0.0f            // Thickness
            );
#endif

            // Array to hold the results
            TArray<UPrimitiveComponent*> OverlappingComponents;

            // Define the object types to query
            TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
            ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

            // Perform the sphere overlap components check
            bool bFoundComponents = UKismetSystemLibrary::SphereOverlapComponents(
                this,
                SphereCenter,
                SightRadius,
                ObjectTypes,
                nullptr,
                TArray<AActor*>{GetPawn()}, // ignore current actor
                OverlappingComponents
            );

            LastFoundComponents = OverlappingComponents;

            return LastFoundComponents;
}

TArray<USlotComponent*> AVillagerAIController::GetVisibleSlots() const
{
    TArray<USlotComponent*> Result;

    TArray<UPrimitiveComponent*> OverlappingComponents = GetVisibleComponents();

    for (UPrimitiveComponent* Component : OverlappingComponents)
    {
        if (USlotComponent* Slot = Cast<USlotComponent>(Component))
        {
            Result.Add(Slot);
        }
    }

    return Result;
}

TArray<USlotComponent*> AVillagerAIController::GetVisibleTalkingSlots() const
{
    TArray<USlotComponent*> Result{};
    TArray<USlotComponent*> VisibleSlots = GetVisibleSlots();

    for (USlotComponent* Slot : VisibleSlots)
    {
        const FGameplayTag TalkingSlotTag = FGameplayTag::RequestGameplayTag(FName("Slots.Villager.TalkingSlot"));
        if (Slot->GameplayTags.HasTag(TalkingSlotTag))
        {
            Result.Add(Slot);
        }
    }

    return VisibleSlots;
}

bool AVillagerAIController::IsThereVisibleTalkingSlot() const
{
    return GetVisibleTalkingSlots().Num() > 0;
}

const TArray<AVillagerPath*> AVillagerAIController::GetVisiblePaths() const
{
    TArray<AVillagerPath*> Result{};

    TArray<UPrimitiveComponent*> OverlappingComponents = GetVisibleComponents();

    for (UPrimitiveComponent* Component : OverlappingComponents)
    {
        if (USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Component))
        {
            if (AVillagerPath* Path = Cast<AVillagerPath>(SplineMesh->GetOwner()))
            {
                Result.Add(Path);
                OverlappingComponents.Remove(Component);
            }
        }
    }

    return Result;
}

bool AVillagerAIController::IsThereVisiblePath() const
{
    return GetVisiblePaths().Num() > 0;
}

EVillagerState AVillagerAIController::DecideWhatToDo()
{
    UE_LOG(LogTemp, Display, TEXT("Energy = %f"), Energy);

    if (Blackboard->GetValueAsObject("Interlocutor") != this)
    {
        return EVillagerState::Talking;
    }

    switch (GetStressState())
    {
    case EStressState::Calm:
        if (UKismetMathLibrary::RandomBoolWithWeight(exp(-Energy)) && !(VillagerState == EVillagerState::WalkingAlongPath && GetCurrentStateDuration() < 30.0f))
        {
            Blackboard->SetValueAsFloat("ChillFor", (-FMath::Pow(Energy, 2) + 1.0f) * 10.0f);
            return EVillagerState::Chilling;
        }
        if (IsThereVisiblePath() && ((Energy > 0.75f && FMath::RandBool()) || (VillagerState == EVillagerState::WalkingAlongPath && GetCurrentStateDuration() < 30.0f)))
        {
            return EVillagerState::WalkingAlongPath;
        }
        if (Energy > 0.3f && IsThereVisibleTalkingSlot() && TryToTalk())
        {
            return EVillagerState::Talking;
        }
        return EVillagerState::WalkingAround;
        break;
    default:
        return EVillagerState::WalkingAround;
    }
}

EStressState AVillagerAIController::GetStressState() const
{
    if (Stress > 0.75f)
    {
        return EStressState::Panic;
    }
    if (Stress > 0.5f)
    {
        return EStressState::Frightened;
    }
    if (Stress > 0.25f)
    {
        return EStressState::Anxious;
    }
    return EStressState::Calm;
}

bool AVillagerAIController::IsReadyToTalk() const
{
    if (Energy > 0.3)
    {
        return true;
    }
    return false;
}

bool AVillagerAIController::TryToTalk()
{
    UBlackboardComponent* BB = GetBlackboardComponent();

    AVillagerAIController* InterlocutorAI = Cast<AVillagerAIController>(BB->GetValueAsObject("Interlocutor"));

    if (InterlocutorAI == this)
    {
        if (AVillageResident* Me = Cast<AVillageResident>(GetPawn()))
        {
            USlotComponent* MySlot = Me->GetTalkingSlot();
            if (!MySlot->IsAvailable())
            {
                return false;
            }

            TArray<USlotComponent*> PotentialInterlocutors{};
            for (USlotComponent* Slot : GetVisibleTalkingSlots())
            {
                if (Slot != MySlot && Slot->IsAvailable())
                {
                    PotentialInterlocutors.Add(Slot);
                }
            }
            if (PotentialInterlocutors.Num() == 0)
            {
                return false;
            }
            USlotComponent* InterlocutorSlot = PotentialInterlocutors[UKismetMathLibrary::RandomIntegerInRange(0, PotentialInterlocutors.Num() - 1)];
            
            if (AVillageResident* Interlocutor = Cast<AVillageResident>(InterlocutorSlot->GetOwner()))
            {
                InterlocutorAI = Cast<AVillagerAIController>(Interlocutor->GetController());
                if (IsValid(InterlocutorAI))
                {
                    if (InterlocutorAI->IsReadyToTalk())
                    {

                        BB->SetValueAsObject("Interlocutor", InterlocutorAI);
                        BB->SetValueAsVector("TargetPoint", InterlocutorSlot->GetComponentLocation());
                        DrawDebugSphere(GetWorld(), InterlocutorSlot->GetComponentLocation(), 25.0f, 12, FColor::Red, false, 5.0);
                        BB->SetValueAsVector("WalkAroundPoint", Interlocutor->GetActorLocation());
                        DrawDebugSphere(GetWorld(), Interlocutor->GetActorLocation(), 25.0f, 12, FColor::Green, false, 5.0);
                        MySlot->Claim(InterlocutorAI);

                        InterlocutorAI->GetBlackboardComponent()->SetValueAsObject("Interlocutor", this);
                        InterlocutorAI->GetBlackboardComponent()->SetValueAsVector("TargetPoint", Interlocutor->GetActorLocation());
                        InterlocutorAI->GetBlackboardComponent()->SetValueAsVector("WalkAroundPoint", InterlocutorSlot->GetComponentLocation());
                        InterlocutorSlot->Claim(this);
                        InterlocutorAI->InterruptCurrentTask();
                        return true;
                    }
                }
            }
        }
    }
    else
    {
        return true;
    }

    return false;



    //if (!IsReadyToTalk()) { return false; }

    //AVillageResident* Requestor = Cast<AVillageResident>(GetPawn());
    //if (!IsValid(Requestor)) { return false; }
    //AVillageResident* Responder = Cast<AVillageResident>(ResponerTalkingSlot->GetOwner());
    //if (!IsValid(Responder)) { return false; }

    //AVillagerAIController* RequestorController = this;
    //AVillagerAIController* ResponderController = Cast<AVillagerAIController>(Responder->GetController());
    //if (!IsValid(ResponderController)) { return false; }

    //if (!ResponderController->IsReadyToTalk()) { return false; }

    //USlotComponent* RequestorSlot = Requestor->GetTalkingSlot();
    //if (!IsValid(RequestorSlot)) { return false; }
    //USlotComponent* ResponderSlot = Responder->GetTalkingSlot();
    //if (!IsValid(ResponderSlot)) { return false; }

    //if (!RequestorSlot->IsAvailable()) { return false; }
    //if (!ResponderSlot->IsAvailable()) { return false; }

    //UBlackboardComponent* RequestorBB = RequestorController->GetBlackboardComponent();
    //if (!IsValid(RequestorBB)) { return false; }
    //UBlackboardComponent* ResponderBB = ResponderController->GetBlackboardComponent();
    //if (!IsValid(ResponderBB)) { return false; }

    //RequestorSlot->Claim(ResponderController);
    //ResponderSlot->Claim(RequestorController);

    //RequestorBB->SetValueAsEnum("Desired State", uint8(EVillagerState::Talking));
    //RequestorBB->SetValueAsVector("TargetPoint", Responder->GetActorLocation());
    //RequestorBB->SetValueAsVector("WalkAroundPoint", ResponderSlot->GetComponentLocation());

    //ResponderBB->SetValueAsEnum("Desired State", uint8(EVillagerState::Talking));
    //ResponderBB->SetValueAsVector("TargetPoint", Responder->GetActorLocation());
    //ResponderBB->SetValueAsVector("WalkAroundPoint", ResponderSlot->GetComponentLocation());

    //RequestorController->InterruptCurrentTask();
    //ResponderController->InterruptCurrentTask();
    //
    //return false;
}

void AVillagerAIController::InterruptCurrentTask()
{
    UBlackboardComponent* BB = GetBlackboardComponent();
    BB->SetValueAsBool("IsInterrupted", true);
    UE_LOG(LogTemp, Warning, TEXT("Set IsInterrupted true"));
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [BB]() {
        BB->SetValueAsBool("IsInterrupted", false);
        UE_LOG(LogTemp, Warning, TEXT("Set IsInterrupted false"));
    }, 0.2f, false);
}
