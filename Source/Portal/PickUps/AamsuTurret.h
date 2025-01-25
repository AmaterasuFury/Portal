// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AamsuPickUpObjectsBase.h"
#include "AamsuTurret.generated.h"


UCLASS()
class PORTAL_API AamsuTurret : public AamsuPickUpObjectsBase
{
	GENERATED_BODY()

public:
	AamsuTurret();
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Aimed Actor Component Parameters")
	/** Recommended value is about 90 degrees */
	float FOVAngle = 90.f;
	
	/** Gets all actors in the FOV, of the owner. Notice: the function checks the actors by their location, if the actor is big, the function might not detect it */
	TArray<AActor*> FOVActorsCheck(float InCheckDistance, ECollisionChannel InCollisionChannel = ECC_Visibility) const;

	TArray<AActor*> GetAllActorsInRadius(float InCheckDistance, ECollisionChannel InCollisionChannel = ECC_Visibility) const;

	/** Checks if an actor is being Out Of Sight by doing LineTraceSingleByChannel */
	bool IsActorCovered(FVector OwnerPosition, AActor* TargetActor, ECollisionChannel InCollisionChannel = ECC_Visibility) const;
};
