// Copyright Epic Games, Inc. All Rights Reserved.

#include "Survival_InventoryGameMode.h"
#include "Survival_InventoryCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASurvival_InventoryGameMode::ASurvival_InventoryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
