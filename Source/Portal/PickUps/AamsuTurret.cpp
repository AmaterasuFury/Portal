// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuTurret.h"
#include "Engine/OverlapResult.h"
#include "Portal/CodeHelpers/amsuGetHelper.h"

AamsuTurret::AamsuTurret()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AamsuTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<APlayerController*> FOVCharacters = FOVCharactersCheck(CheckDistance);

	OnActiveMode(bIsInActiveRadius);
}

void AamsuTurret::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllersInWorld = amsuGetHelper::GetAllPlayerControllers(GetWorld());

	SetActorTickEnabled(true);
}

// todo fix description
/** If u want to make this function more accurate u would have to implement a logic to additionally trace a linetrace to actors angle, but it would cost you more of the performance, of course */
TArray<APlayerController*> AamsuTurret::FOVCharactersCheck(float InCheckDistance) const
{
	TArray<APlayerController*> PlayerControllersInRadius;
	if (PlayerControllersInWorld.IsEmpty())
	{
		return PlayerControllersInWorld;
	}

	const FVector TurretLocation = GetOwner()->GetTargetLocation();
	
	
	for (TObjectPtr<APlayerController> PlayerController : PlayerControllersInWorld)
	{
		if (FVector::Dist(PlayerController->GetTargetLocation(), TurretLocation) <= InCheckDistance)
		{
			PlayerControllersInRadius.Add(PlayerController);
		}
	}
	if (PlayerControllersInRadius.IsEmpty())
	{
		return PlayerControllersInRadius;
	}

	TArray<APlayerController*> PlayerControllersInFOV;
	
	const FVector NormalizedOwnerForward = GetOwner()->GetActorForwardVector();
	TArray<FVector> NormalizedDirectionToTargets;
	
	for (APlayerController* PlayerController: PlayerControllersInRadius)
	{
		if (IsValid(PlayerController))
		{
			const FVector NormalizedDirectionToTarget = (PlayerController->GetTargetLocation() - TurretLocation).GetSafeNormal();

			const float DotProduct = FVector::DotProduct(NormalizedOwnerForward, NormalizedDirectionToTarget);
			
			const float Threshold = FMath::Cos(FMath::DegreesToRadians(FOVAngle / 2));
			
			if (DotProduct >= Threshold && !IsCharacterCovered(TurretLocation, PlayerController)) 
			{
				PlayerControllersInFOV.Add(PlayerController);
			}
		}
	}
	
	return PlayerControllersInFOV;
}


bool AamsuTurret::IsCharacterCovered(const FVector& OwnerPosition, AActor* TargetActor, ECollisionChannel InCollisionChannel) const
{
	FHitResult HitResult;
	FCollisionQueryParams FCollisionQueryParams;

	FCollisionQueryParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, OwnerPosition, TargetActor->GetActorLocation(), InCollisionChannel, FCollisionQueryParams );
	
	return HitResult.GetActor() != TargetActor;
}

void AamsuTurret::OnActiveMode(bool bActivate)
{
	//TArray<AActor*> AllFOVActors = FOVCharactersCheck(CheckDistance, ECC_Pawn);
//
	//TArray<ACharacter*> FOVCharacters;
	//for (AActor* FOVActor : FOVActors)
	//{
	//	 
	//}
}

