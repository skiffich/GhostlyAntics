// Fill out your copyright notice in the Description page of Project Settings.


#include "VillagerPath.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AVillagerPath::AVillagerPath()
{
	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	PathSpline->SetMobility(EComponentMobility::Static);
    RootComponent = PathSpline;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (StaticMeshAsset.Succeeded())
	{
		SplineMesh = StaticMeshAsset.Object;
	}

    PathVisibility = false;

    PathWidthScale = 1.0f;
}

USplineComponent* AVillagerPath::GetPathSpline() const
{
	return PathSpline;
}

float AVillagerPath::GetWidth() const
{
	return SplineMesh->GetBounds().GetBox().GetSize().Y * PathWidthScale;
}

void AVillagerPath::AddSplineMesh()
{
    const float SplineLength = PathSpline->GetSplineLength();
    const float SplineMeshLength = SplineMesh->GetBounds().GetBox().GetSize().X;

    int NumTiles = SplineLength / SplineMeshLength;

    UE_LOG(LogTemp, Display, TEXT("%f"), GetWidth());

    for (int i = 0; i < NumTiles; ++i)
    {
        FVector StartPos, StartTangent, EndPos, EndTangent;
        if (i == NumTiles - 1)
        {
            StartPos = PathSpline->GetLocationAtDistanceAlongSpline(i * SplineMeshLength, ESplineCoordinateSpace::World);
            StartTangent = PathSpline->GetTangentAtDistanceAlongSpline(i * SplineMeshLength, ESplineCoordinateSpace::World);
            EndPos = PathSpline->GetLocationAtDistanceAlongSpline(SplineLength, ESplineCoordinateSpace::World);
            EndTangent = PathSpline->GetTangentAtDistanceAlongSpline(SplineLength, ESplineCoordinateSpace::World);
        }
        else
        {
            StartPos = PathSpline->GetLocationAtDistanceAlongSpline(i * SplineMeshLength, ESplineCoordinateSpace::World);
            StartTangent = PathSpline->GetTangentAtDistanceAlongSpline(i * SplineMeshLength, ESplineCoordinateSpace::World);
            EndPos = PathSpline->GetLocationAtDistanceAlongSpline((i + 1) * SplineMeshLength, ESplineCoordinateSpace::World);
            EndTangent = PathSpline->GetTangentAtDistanceAlongSpline((i + 1) * SplineMeshLength, ESplineCoordinateSpace::World);
        }

        // Create a new spline mesh component
        USplineMeshComponent* NewSplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
        NewSplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
        NewSplineMesh->AttachToComponent(PathSpline, FAttachmentTransformRules::KeepWorldTransform);
        NewSplineMesh->SetStaticMesh(SplineMesh);
        NewSplineMesh->SetStartScale(FVector2D(PathWidthScale, 1.0f));
        NewSplineMesh->SetEndScale(FVector2D(PathWidthScale, 1.0f));
        NewSplineMesh->SetVisibility(PathVisibility);
        NewSplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
        NewSplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        NewSplineMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
        NewSplineMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
        NewSplineMesh->RegisterComponent();
    }
}