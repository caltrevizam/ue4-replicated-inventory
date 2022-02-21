// Copyright Epic Games, Inc. All Rights Reserved.

#include "Framework/PathToRagnarokGameMode.h"
#include "Player/PathToRagnarokPlayerController.h"
#include "UObject/ConstructorHelpers.h"

APathToRagnarokGameMode::APathToRagnarokGameMode()
{
	PlayerControllerClass = APathToRagnarokPlayerController::StaticClass();
}