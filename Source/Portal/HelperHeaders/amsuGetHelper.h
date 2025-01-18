#pragma once

#include "CoreMinimal.h"

// TODO upgrade and refactor - use templates 
inline APlayerController* GetPlayerController(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}
	const APawn* InteractingPawn = Cast<APawn>(Actor);
	return Cast<APlayerController>(InteractingPawn->GetController());
}