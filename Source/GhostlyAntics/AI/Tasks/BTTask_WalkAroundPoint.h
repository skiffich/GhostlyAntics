// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_WalkAroundPoint.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API UBTTask_WalkAroundPoint : public UBTTask_MoveTo
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
	UBTTask_WalkAroundPoint();

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool GetRandomPointInRadius(const UBehaviorTreeComponent& OwnerComp);

	FVector DestinationLocation;

	EBTNodeResult::Type ExecutionResult;
};
