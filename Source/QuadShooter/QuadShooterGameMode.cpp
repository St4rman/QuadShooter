// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuadShooterGameMode.h"
#include "QuadShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQuadShooterGameMode::AQuadShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
