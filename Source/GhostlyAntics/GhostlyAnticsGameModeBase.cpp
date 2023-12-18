// Copyright Epic Games, Inc. All Rights Reserved.


#include "GhostlyAnticsGameModeBase.h"

AGhostlyAnticsGameModeBase::AGhostlyAnticsGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Ghost/BP_GhostCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
