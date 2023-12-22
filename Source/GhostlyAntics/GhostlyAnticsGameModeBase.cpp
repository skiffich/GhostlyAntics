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

	// set default player controller class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> GhostControllerClass(TEXT("/Game/Controllers/BP_GhostController"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		PlayerControllerClass = GhostControllerClass.Class;
	}
}
