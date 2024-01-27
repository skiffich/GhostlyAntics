// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_GetTargetAlongPath.h"
#include "Controllers/VillagerAIController.h"
#include "Actors/Miscellaneous/VillagerPath.h"
#include "Actors/Characters/VillageResident.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetTargetAlongPath::UBTTask_GetTargetAlongPath(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Get Target Along Path";
}

EBTNodeResult::Type UBTTask_GetTargetAlongPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AVillagerAIController* AIController = Cast<AVillagerAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController is invalid"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	AIController->SetState(EVillagerState::WalkingAlongPath);

	if (!AIController->IsThereVisiblePath())
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no visible paths"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	AVillagerPath* Path = AIController->GetVisiblePaths()[FMath::RandRange(0, AIController->GetVisiblePaths().Num() - 1)];

	if (!IsValid(Path))
	{
		UE_LOG(LogTemp, Warning, TEXT("Path is invalid"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	USplineComponent* PathSpline = Path->GetPathSpline();

	if (!IsValid(PathSpline))
	{
		UE_LOG(LogTemp, Warning, TEXT("PathSpline is invalid"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	FVector VillagerLocation = AIController->GetPawn()->GetActorLocation();

	//
	// change direction if necessary
	float CurrentDistanceAlongPath = PathSpline->GetDistanceAlongSplineAtSplineInputKey(PathSpline->FindInputKeyClosestToWorldLocation(VillagerLocation));
	bool NeedToChangeDirection = AIController->GetCurrentStateDuration() > CHANGE_DIRECTION_FREQUENSY;
	float NoramalizedCurrDist = CurrentDistanceAlongPath / PathSpline->GetSplineLength();
	float ProbToChangeDirection = FMath::Pow((NoramalizedCurrDist - 0.5f), 2.0f) * 0.25f;
	NeedToChangeDirection = PathSpline->IsClosedLoop() ? NeedToChangeDirection : (NeedToChangeDirection || UKismetMathLibrary::RandomBoolWithWeight(ProbToChangeDirection));
	if (NeedToChangeDirection)
	{
		AIController->SetPathFollowingDirection(FMath::RandBool());
	}

	//
	// Calculate Target Distance
	float Delta = FMath::RandRange(100.0f, AIController->GetSightRadius());
	Delta = Delta * (AIController->GetPathFollowingDirection() ? 1.0 : -1.0);
	float TargetDistance = CurrentDistanceAlongPath + Delta;
	if (PathSpline->IsClosedLoop())
	{
		if (TargetDistance < 0)
		{
			TargetDistance = PathSpline->GetSplineLength() + TargetDistance;
		}
		else if (TargetDistance > PathSpline->GetSplineLength())
		{
			TargetDistance = TargetDistance - PathSpline->GetSplineLength();
		}
	}
	else
	{
		TargetDistance = UKismetMathLibrary::FClamp(TargetDistance, 0.0f, PathSpline->GetSplineLength());
	}

	// Calculate Target Location
	FVector RightVectorAtTargetDisnace = PathSpline->GetRightVectorAtDistanceAlongSpline(TargetDistance, ESplineCoordinateSpace::World);
	FVector LocationAtTargetDisnace = PathSpline->GetLocationAtDistanceAlongSpline(TargetDistance, ESplineCoordinateSpace::World);
	FVector TargetLocation = LocationAtTargetDisnace + (RightVectorAtTargetDisnace * FMath::RandRange(0 - Path->GetWidth() / 2, Path->GetWidth() / 2));

	DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 10, FColor::Green, false, 3.0f, 0, 1.0f);

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	BlackboardComp->SetValueAsVector(TargetPointKey.SelectedKeyName, TargetLocation);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
