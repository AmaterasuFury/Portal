#pragma once

#include "Portal/CodeHelpers/amsuGetHelper.h"

APlayerController* amsuGetHelper::GetPlayerController(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	const APawn* InteractingPawn = Cast<APawn>(Actor);
	if (!IsValid(InteractingPawn))
	{
		return nullptr;
	}
	return Cast<APlayerController>(InteractingPawn->GetController());
}

TArray<APlayerController*> amsuGetHelper::GetAllPlayerControllers(const UWorld* World)
{
	TArray<APlayerController*> PlayerControllers;

	if (!World)
	{
		return PlayerControllers;
	}

	// Iterate through all player controllers in the world
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (IsValid(PlayerController))
		{
			PlayerControllers.Add(PlayerController);
		}
	}

	return PlayerControllers;
}


