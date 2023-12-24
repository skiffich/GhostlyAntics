// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_WalkToPoint.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API UBTTask_WalkToPoint : public UBTTask_MoveTo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	float WalkingRadius{ 500.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	float MinDistance{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	int MaxAttemptsToFindPoint{ 5 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick", meta = (AllowPrivateAccess = true))
	float TickTime{ 1.0f };

public:
	UBTTask_WalkToPoint();

protected:
	/*virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual FString GetStaticDescription() const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;*/

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool GetRandomPointInRadius(const UBehaviorTreeComponent& OwnerComp);

	FVector DestinationLocation;

	EBTNodeResult::Type ExecutionResult;
};
