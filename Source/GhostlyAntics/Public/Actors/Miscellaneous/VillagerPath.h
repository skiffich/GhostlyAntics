// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "VillagerPath.generated.h"

UCLASS()
class GHOSTLYANTICS_API AVillagerPath : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AVillagerPath();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USplineComponent* GetPathSpline() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetWidth() const;

protected:
	UFUNCTION(BlueprintCallable)
	void AddSplineMesh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineComponent* PathSpline;

	UPROPERTY(EditInstanceOnly)
	float PathWidthScale;

	UPROPERTY(EditInstanceOnly)
	bool PathVisibility;

private:
	UStaticMesh* SplineMesh;

};
