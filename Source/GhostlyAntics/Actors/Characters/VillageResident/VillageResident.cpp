// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageResident.h"

// Sets default values
AVillageResident::AVillageResident()
{
 	
}

// Called when the game starts or when spawned
void AVillageResident::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		
	}
	
}

// Called every frame
void AVillageResident::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}