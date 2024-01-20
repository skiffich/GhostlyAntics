// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "VillagerPath.generated.h"

UCLASS()
class GHOSTLYANTICS_API AVillagerPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVillagerPath();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineComponent* Path;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
