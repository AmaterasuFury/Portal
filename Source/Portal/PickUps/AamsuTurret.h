// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AamsuPickUpObjectsBase.h"
#include "AamsuTurret.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(amsuTurret, Log, Warning)

UCLASS()
class PORTAL_API AamsuTurret : public AamsuPickUpObjectsBase
{
	GENERATED_BODY()

public:
	AamsuTurret();
	
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	
	/** Gets all characters in the FOV, of the owner*/
	TArray<APawn*> GetPawnsInFOV(float InCheckDistance) const;
	
	/** Checks if an actor is being Out Of Sight by doing LineTraceSingleByChannel */
	bool IsActorCovered(const FVector& OwnerPosition, AActor* TargetActor, ECollisionChannel InCollisionChannel = ECC_Visibility) const;

	/** Scans for the character in the provided FOVAngle and shot if finds */
	void TickActiveMode(float InDeltaTime);

	void DeactivateTurret();
	
private:
	UPROPERTY(Transient)
	TObjectPtr<APawn> TargetPawn = nullptr;

	UPROPERTY(EditAnywhere, Category = "Turret parameters")
	/** Recommended value is about 90 degrees */
	float FOVAngle = 90.f;

	UPROPERTY(EditAnywhere, Category = "Turret parameters")
	float CheckDistance = 3000.f;

	//** The damage of the turret  */
	UPROPERTY(EditAnywhere, Category = "Turret parameters")
	float TurretDamage = 90.f;
};
