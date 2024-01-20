// Fill out your copyright notice in the Description page of Project Settings.


#include "VillagerPath.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AVillagerPath::AVillagerPath()
{
	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
    Path->SetMobility(EComponentMobility::Static);
    RootComponent = Path;
}

// Called when the game starts or when spawned
void AVillagerPath::BeginPlay()
{
	Super::BeginPlay();
	
}