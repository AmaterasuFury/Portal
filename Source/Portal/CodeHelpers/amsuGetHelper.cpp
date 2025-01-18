#pragma once

#include "Portal/CodeHelpers/amsuGetHelper.h"

APlayerController* amsuGetHelper::GetPlayerController(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}
	const APawn* InteractingPawn = Cast<APawn>(Actor);
	return Cast<APlayerController>(InteractingPawn->GetController());
}


