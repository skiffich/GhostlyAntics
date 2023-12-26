// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_InviteTalking.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API UBTTask_InviteTalking : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_InviteTalking();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
