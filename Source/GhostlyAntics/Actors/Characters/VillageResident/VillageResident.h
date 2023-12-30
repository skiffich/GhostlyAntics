// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Characters/GACharacter/GACharacter.h"
#include "VillageResident.generated.h"

UENUM(BlueprintType)
enum class EVillagerState : uint8
{
	None UMETA(DisplayName = "None"),
	Walking UMETA(DisplayName = "Walking"),
	TalkingRequest UMETA(DisplayName = "TalkingRequest"),
	Talking UMETA(DisplayName = "Talking")
};

UCLASS()
class GHOSTLYANTICS_API AVillageResident : public AGACharacter
{
	GENERATED_BODY()

	/** Chance to talk */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Talking", meta = (AllowPrivateAccess = "true"))
	float ChanceToTalk { 0.5f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Talking", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimationAsset>> TalkingAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Talking", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimationAsset>> InviteToTalkAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Talking", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USoundCue>> TalkingSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Talking", meta = (AllowPrivateAccess = "true"))
	USoundCue* SoundInviteTalking;

public:
	// Sets default values for this character's properties
	AVillageResident();

	FORCEINLINE bool DoesWantToTalk();

	FORCEINLINE bool IsBusy();

	UFUNCTION(BlueprintPure, Category = "Talking")
	FORCEINLINE bool IsTalking() { return VillagerState == EVillagerState::Talking;	}

	bool RequestConversation(AVillageResident* initiator, AVillageResident* responder);

	void StartTalking();
	void FinishTalking();
private:
	FTimerDelegate TalkingTimerDelegate;
	FTimerHandle TalkingTimerHandle;
	FDelegateHandle VillagerPathMoveToDelegate;;

protected:

	// To add mapping context
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	EVillagerState VillagerState;
};
