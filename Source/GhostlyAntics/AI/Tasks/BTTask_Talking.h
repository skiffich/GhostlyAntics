// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Talking.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API UBTTask_Talking : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(Category = Node, EditAnywhere)
	float TalkingTime{ 10.0f };

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> MyOwnerComp;

	FTimerDelegate TalkingTimerDelegate;
	FTimerHandle TalkingTimerHandle;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void OnTalkingTimerDone();
};
