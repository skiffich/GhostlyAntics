// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Chill.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API UBTTask_Chill : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_Chill(const FObjectInitializer& ObjectInitializer);

	float WaitTime;

	float RandomDeviation;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowedTypes = "float"))
	FBlackboardKeySelector WaitTimeKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

protected:

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
