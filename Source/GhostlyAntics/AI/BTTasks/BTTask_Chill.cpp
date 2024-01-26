// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chill.h"
#include "AIController.h"
#include "../VillagerAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

//#include UE_INLINE_GENERATED_CPP_BY_NAME(UBTTask_Chill)

UBTTask_Chill::UBTTask_Chill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Chill";
	WaitTime = 5.0f;
	bTickIntervals = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();

	WaitTimeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Chill, WaitTimeKey));
}

EBTNodeResult::Type UBTTask_Chill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AVillagerAIController* AIController = Cast<AVillagerAIController>(OwnerComp.GetAIOwner()))
	{
		AIController->SetState(EVillagerState::Chilling);

		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp->IsValidKey(BlackboardComp->GetKeyID(WaitTimeKey.SelectedKeyName)))
		{
			WaitTime = BlackboardComp->GetValueAsFloat(WaitTimeKey.SelectedKeyName);
		}
	}

	RandomDeviation = WaitTime * 0.25;

	const float RemainingWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));
	SetNextTickTime(NodeMemory, RemainingWaitTime);

	return EBTNodeResult::InProgress;
}

void UBTTask_Chill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Using the SetNextTickTime in ExecuteTask we are certain we are only getting ticked when the wait is finished
	ensure(GetSpecialNodeMemory<FBTTaskMemory>(NodeMemory)->NextTickRemainingTime <= 0.f);

	// continue execution from this node
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

FString UBTTask_Chill::GetStaticDescription() const
{
	if (FMath::IsNearlyZero(RandomDeviation))
	{
		return FString::Printf(TEXT("%s: %.1fs"), *Super::GetStaticDescription(), WaitTime);
	}
	else
	{
		return FString::Printf(TEXT("%s: %.1f+-%.1fs"), *Super::GetStaticDescription(), WaitTime, RandomDeviation);
	}
}

void UBTTask_Chill::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	FBTTaskMemory* TaskMemory = GetSpecialNodeMemory<FBTTaskMemory>(NodeMemory);
	if (TaskMemory->NextTickRemainingTime)
	{
		Values.Add(FString::Printf(TEXT("remaining: %ss"), *FString::SanitizeFloat(TaskMemory->NextTickRemainingTime)));
	}
}

#if WITH_EDITOR

FName UBTTask_Chill::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.Wait.Icon");
}

#endif	// WITH_EDITOR
