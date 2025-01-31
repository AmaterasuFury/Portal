// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuTurret.h"
#include "Engine/OverlapResult.h"
#include "Portal/CodeHelpers/amsuGetHelper.h"

DEFINE_LOG_CATEGORY(amsuTurret)

AamsuTurret::AamsuTurret()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AamsuTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	OnActiveMode(bIsInActiveRadius);
}

void AamsuTurret::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(true);

	PlayerControllersInWorld = amsuGetHelper::GetAllPlayerControllers(GetWorld());
}


/** Returns all pawns in the FOV that are owned by PlayerController*/
TArray<APawn*> AamsuTurret::FOVCharactersCheck(float InCheckDistance) const
{
	TArray<APawn*> PlayerPawnsInRadius;
	if (PlayerControllersInWorld.IsEmpty())
	{
		return PlayerPawnsInRadius;
	}
	
	if (!IsValid(RootComponent))
	{
		UE_LOG(amsuTurret, Log, TEXT("WARNING, the RootComponet of the amsuTurret is not valid"))
	}
	
	const FVector TurretLocation = RootComponent->GetComponentLocation();;
	
	for (const TObjectPtr<APlayerController> PlayerController : PlayerControllersInWorld)
	{
		APawn* PlayerControllerPawn = PlayerController->GetPawn();
		if (!IsValid(PlayerControllerPawn))
		{
			continue;
		}
		if (FVector::Dist(PlayerControllerPawn->GetActorLocation(), TurretLocation) <= InCheckDistance)
		{
			PlayerPawnsInRadius.Add(PlayerControllerPawn);
		}
	}
	if (PlayerPawnsInRadius.IsEmpty())
	{
		return PlayerPawnsInRadius;
	}

	TArray<APawn*> PlayerPawnsInFOV;
	
	const FVector NormalizedOwnerForward = RootComponent->GetForwardVector();
	TArray<FVector> NormalizedDirectionToTargets;
	
	for (APawn* PlayerPawn: PlayerPawnsInRadius)
	{
		if (IsValid(PlayerPawn))
		{
			const FVector NormalizedDirectionToTarget = (PlayerPawn->GetActorLocation() - TurretLocation).GetSafeNormal();

			const float DotProduct = FVector::DotProduct(NormalizedOwnerForward, NormalizedDirectionToTarget);
			
			const float Threshold = FMath::Cos(FMath::DegreesToRadians(FOVAngle / 2));
			
			if (DotProduct >= Threshold && !IsActorCovered(TurretLocation, PlayerPawn, ECC_Pawn)) 
			{
				PlayerPawnsInFOV.Add(PlayerPawn);
			}
		}
	}
	
	return PlayerPawnsInFOV;
}


bool AamsuTurret::IsActorCovered(const FVector& OwnerPosition, AActor* TargetActor, ECollisionChannel InCollisionChannel) const
{
	FHitResult HitResult;
	FCollisionQueryParams FCollisionQueryParams;
	
	FCollisionQueryParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, OwnerPosition, TargetActor->GetActorLocation(), InCollisionChannel, FCollisionQueryParams);

#if ENABLE_DRAW_DEBUG && 0
	if (HitResult.GetActor() == TargetActor)
	{
		DrawDebugLine(GetWorld(), OwnerPosition, TargetActor->GetActorLocation(), FColor::Green,
		false, 0.1f, 0, 4.f);
	}
#endif
	
	return HitResult.GetActor() != TargetActor;
}

void AamsuTurret::OnActiveMode(bool bActivate)
{
	TArray<APawn*> FOVCharacters = FOVCharactersCheck(CheckDistance);
	
	if (FOVCharacters.IsEmpty())
	{
		return;
	}
	if (FOVCharacters.Num() == 1)
	{
		TargetPawn = FOVCharacters[0];
	}
	else
	{
		if (!FOVCharacters.Contains(TargetPawn))
		{
			TargetPawn = FOVCharacters[FMath::RandRange(0,FOVCharacters.Num())];
		}
	}

#if ENABLE_DRAW_DEBUG && 1
	DrawDebugLine(GetWorld(), RootComponent->GetComponentLocation(), TargetPawn->GetActorLocation(), FColor::Red,
		false, 0.1f, 0, 4.f);
	
#endif
	
	//Shoot(TargetPawn);
}

void AamsuTurret::Shoot(APawn* Pawn)
{
	
	
	// TODO first shoot a debuglinetrace
#if ENABLE_DRAW_DEBUG && 0
	DrawDebugLine(GetWorld(), RootComponent->GetComponentLocation(), TargetActor->GetActorLocation(), FColor::Green,
		false, 0.1f, 0, 4.f);
	
#endif

	
}

