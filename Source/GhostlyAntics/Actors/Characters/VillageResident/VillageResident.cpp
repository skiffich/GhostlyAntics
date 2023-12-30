// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageResident.h"
#include "../../../AI/VillagerAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Components/CapsuleComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include <functional>


// Sets default values
AVillageResident::AVillageResident()
{
	AIControllerClass = AVillagerAIController::StaticClass();

	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	VillagerState = EVillagerState::None;

	TalkingTimerDelegate = FTimerDelegate::CreateUObject(this, &AVillageResident::StartTalking);
}

bool AVillageResident::DoesWantToTalk()
{
	return !IsBusy() && FMath::RandRange(0.0f, 1.0f) < ChanceToTalk;
}

bool AVillageResident::IsBusy()
{
	return VillagerState != EVillagerState::None && VillagerState != EVillagerState::Walking;
}

bool AVillageResident::RequestConversation(AVillageResident* initiator, AVillageResident* responder)
{
	AVillagerAIController* AIController = Cast<AVillagerAIController>(GetController());

	if (!IsValid(AIController))
	{
		return false;
	}

	if (initiator == this)  // As initiator
	{
		VillagerState = EVillagerState::TalkingRequest;
		AIController->SetVillagerAIState(EVillagerAIState::None);
		AIController->StopMovement();

		if (responder->RequestConversation(initiator, responder))
		{
			// Say hey
			if (SoundInviteTalking)
			{
				AudioComponent->SetSound(SoundInviteTalking);
				AudioComponent->Play();
			}

			// wave hand
			if (USkeletalMeshComponent* SkelMesh = GetMesh())
			{
				EAnimationMode::Type PreviousAnimationMode = SkelMesh->GetAnimationMode();
				if (InviteToTalkAnimations.Num() > 0)
				{
					UAnimationAsset* InviteToTalkAnimation = InviteToTalkAnimations[FMath::RandRange(0, InviteToTalkAnimations.Num() - 1)];
					SkelMesh->PlayAnimation(InviteToTalkAnimation,false);
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, [SkelMesh, PreviousAnimationMode]()
					{
						SkelMesh->SetAnimationMode(PreviousAnimationMode);
					}, InviteToTalkAnimation->GetPlayLength(), false);
				}
			}

			return true;
		}
	}
	else if (responder == this) // As responder
	{
		if (DoesWantToTalk())
		{
			VillagerState = EVillagerState::TalkingRequest;
			AIController->SetVillagerAIState(EVillagerAIState::None);

			AIController->StopMovement();

			// move to initiator
			AIController->MoveToActor(initiator , initiator->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.75f);
			VillagerPathMoveToDelegate = AIController->GetPathFollowingComponent()->OnRequestFinished.AddLambda([this, AIController, initiator, responder](FAIRequestID RequestID, const FPathFollowingResult& Result)
			{
				if (Result.IsSuccess())
				{
					UE_LOG(LogTemp, Display, TEXT("%s has come to %s and starting to talk"), *responder->GetName(), *initiator->GetName());

					if (AVillagerAIController* inititorController = Cast<AVillagerAIController>(initiator->GetController()))
					{
						// start talking
						inititorController->BeginTalkingWith(responder);
					}

					// start talking
					AIController->BeginTalkingWith(initiator);
					AIController->GetPathFollowingComponent()->OnRequestFinished.Remove(VillagerPathMoveToDelegate);
				}
			});
			return true;
		}
	}

	AIController->SetVillagerAIState(EVillagerAIState::Walking);
	VillagerState = EVillagerState::Walking;
	return false;
}

void AVillageResident::StartTalking()
{
	VillagerState = EVillagerState::Talking;
	if (TalkingSounds.Num() > 0)
	{
		USoundCue* TalkingSound = TalkingSounds[FMath::RandRange(0, TalkingSounds.Num() - 1)].Get();
		if (TalkingSound)
		{
			AudioComponent->SetSound(TalkingSound);
			float SoundDuration = TalkingSound->GetDuration();
			GetWorld()->GetTimerManager().SetTimer(TalkingTimerHandle, TalkingTimerDelegate, SoundDuration, /*bLoop=*/false);
			AudioComponent->Play();
		}
	}
}

void AVillageResident::FinishTalking()
{
	AudioComponent->Stop();
	if (TalkingTimerHandle.IsValid())
	{
		TalkingTimerHandle.Invalidate();
	}

	if (AVillagerAIController* CurrentController = Cast<AVillagerAIController>(GetController()))
	{
		CurrentController->SetVillagerAIState(EVillagerAIState::Walking);
	}

	VillagerState = EVillagerState::Walking;
}

// Called when the game starts or when spawned
void AVillageResident::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

// Called every frame
void AVillageResident::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
