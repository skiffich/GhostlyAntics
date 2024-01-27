// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetTargetAlongPath.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API UBTTask_GetTargetAlongPath : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_GetTargetAlongPath(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	const float CHANGE_DIRECTION_FREQUENSY = 20.0f;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowedTypes = "Vector"))
	FBlackboardKeySelector TargetPointKey;
};
