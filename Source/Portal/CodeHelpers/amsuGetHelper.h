#pragma once

#include "CoreMinimal.h"

// TODO think about an upgrade and refactor - use templates

struct amsuGetHelper
{
	static APlayerController* GetPlayerController(AActor* Actor);

	static TArray<APlayerController*> GetAllPlayerControllers(const UWorld* World);
};
