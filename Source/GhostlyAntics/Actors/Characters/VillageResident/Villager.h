// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Characters/GACharacter/GACharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Villager.generated.h"

UCLASS()
class GHOSTLYANTICS_API AVillager : public AGACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVillager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* MedievalMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
