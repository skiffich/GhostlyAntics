// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "VillagerBTT_GoRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTLYANTICS_API UVillagerBTT_GoRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	float SearchRadius{ 500.0f };
	
public:
	UVillagerBTT_GoRandomLocation();

protected:
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual FString GetStaticDescription() const override;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* ModeMemmory) override;
};
