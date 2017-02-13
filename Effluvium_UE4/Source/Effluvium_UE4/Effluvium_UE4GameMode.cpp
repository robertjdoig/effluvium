// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Effluvium_UE4.h"
#include "Effluvium_UE4GameMode.h"
#include "Effluvium_UE4HUD.h"
#include "Effluvium_UE4Character.h"

AEffluvium_UE4GameMode::AEffluvium_UE4GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AEffluvium_UE4HUD::StaticClass();
}
