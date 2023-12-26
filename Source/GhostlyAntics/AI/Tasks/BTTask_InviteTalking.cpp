// Fill out your copyright notice in the Description page of Project Settings.


#include "../../AI/Tasks/BTTask_InviteTalking.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_InviteTalking::UBTTask_InviteTalking()
{
	NodeName = TEXT("BTTask_InviteTalking");
}

EBTNodeResult::Type UBTTask_InviteTalking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsBool("InviteToTalk", false);
		}
	}
	return EBTNodeResult::Type();
}
