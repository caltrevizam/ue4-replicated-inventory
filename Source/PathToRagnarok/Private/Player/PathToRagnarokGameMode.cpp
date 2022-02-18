// Copyright Epic Games, Inc. All Rights Reserved.

#include "Framework/PathToRagnarokGameMode.h"
#include "Player/PathToRagnarokPlayerController.h"
#include "UObject/ConstructorHelpers.h"

APathToRagnarokGameMode::APathToRagnarokGameMode()
{
	PlayerControllerClass = APathToRagnarokPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
		DefaultPawnClass = PlayerPawnBPClass.Class;
}