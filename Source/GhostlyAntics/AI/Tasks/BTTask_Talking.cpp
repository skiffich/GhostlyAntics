// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Talking.h"
#include "Engine/World.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"
#include "../../Actors/Characters/VillageResident/VillageResident.h"
#include "../VillagerAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_Talking)

//----------------------------------------------------------------------//
// UBTTask_Talking
//----------------------------------------------------------------------//
UBTTask_Talking::UBTTask_Talking(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Talking";
	// instantiating to be able to use Timers
	bCreateNodeInstance = true;

	TalkingTimerDelegate = FTimerDelegate::CreateUObject(this, &UBTTask_Talking::OnTalkingTimerDone);
}

EBTNodeResult::Type UBTTask_Talking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// reset timer handle
	TalkingTimerHandle.Invalidate();
	MyOwnerComp = &OwnerComp;

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (!IsValid(OwnerController))
	{
		return EBTNodeResult::Failed;
	}

	AVillageResident* villager = Cast<AVillageResident>(OwnerController->GetPawn());
	if (!IsValid(villager))
	{
		return EBTNodeResult::Failed;
	}

	OwnerController->GetWorld()->GetTimerManager().SetTimer(TalkingTimerHandle, TalkingTimerDelegate, TalkingTime, /*bLoop=*/false);

	villager->StartTalking();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Talking::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (!IsValid(OwnerController))
	{
		return EBTNodeResult::Aborted;
	}

	if (TalkingTimerHandle.IsValid())
	{
		OwnerController->GetWorld()->GetTimerManager().ClearTimer(TalkingTimerHandle);
	}

	TalkingTimerHandle.Invalidate();

	return EBTNodeResult::Aborted;
}

void UBTTask_Talking::OnTalkingTimerDone()
{
	AAIController* OwnerController = MyOwnerComp.Get()->GetAIOwner();
	if (!IsValid(OwnerController))
	{
		FinishLatentTask(*MyOwnerComp.Get(), EBTNodeResult::Failed);
		return;
	}

	AVillageResident* villager = Cast<AVillageResident>(OwnerController->GetPawn());
	if (!IsValid(villager))
	{
		FinishLatentTask(*MyOwnerComp.Get(), EBTNodeResult::Failed);
		return;
	}

	villager->FinishTalking();

	FinishLatentTask(*MyOwnerComp.Get(), EBTNodeResult::Succeeded);
}


