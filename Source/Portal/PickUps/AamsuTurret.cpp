// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuTurret.h"

#include "EngineUtils.h"
#include "Portal/CharactersAndController/PortalCharacter.h"
#include "Portal/CodeHelpers/amsuGetHelper.h"

DEFINE_LOG_CATEGORY(amsuTurret)

AamsuTurret::AamsuTurret()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AamsuTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	TickActiveMode(DeltaTime);
}

void AamsuTurret::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(true);
}


/** Returns all pawns in the FOV that are owned by PlayerController*/
TArray<APawn*> AamsuTurret::GetPawnsInFOV(float InCheckDistance) const
{
	TArray<APawn*> PlayerPawnsInRadius;
	
	if (!IsValid(RootComponent))
	{
		UE_LOG(amsuTurret, Log, TEXT("WARNING, the RootComponet of the amsuTurret is not valid"))
	}
	
	const FVector TurretLocation = RootComponent->GetComponentLocation();
	
	for (const TObjectPtr<APawn> PawnInWorld : TActorRange<APawn>(GetWorld()))
	{
		if (!IsValid(PawnInWorld))
		{
			continue;
		}
		if (FVector::Dist(PawnInWorld->GetActorLocation(), TurretLocation) <= InCheckDistance)
		{
			PlayerPawnsInRadius.Add(PawnInWorld);
		}
	}
	if (PlayerPawnsInRadius.IsEmpty())
	{
		return PlayerPawnsInRadius;
	}

	TArray<APawn*> PlayerPawnsInFOV;
	
	const FVector NormalizedOwnerForward = RootComponent->GetForwardVector();
	TArray<FVector> NormalizedDirectionToTargets;
	const float Threshold = FMath::Cos(FMath::DegreesToRadians(FOVAngle / 2));

	
	for (APawn* PlayerPawn: PlayerPawnsInRadius)
	{
		if (!IsValid(PlayerPawn))
		{
			continue;
		}
		
		const FVector NormalizedDirectionToTarget = (PlayerPawn->GetActorLocation() - TurretLocation).GetSafeNormal();

		const float DotProduct = FVector::DotProduct(NormalizedOwnerForward, NormalizedDirectionToTarget);
		
		if (DotProduct >= Threshold && !IsActorCovered(TurretLocation, PlayerPawn, ECC_Pawn)) 
		{
			PlayerPawnsInFOV.Add(PlayerPawn);
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

void AamsuTurret::TickActiveMode(float InDeltaTime)
{
	
	TArray<APawn*> FOVCharacters = GetPawnsInFOV(CheckDistance);
	
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

	//** Shoot */
#if ENABLE_DRAW_DEBUG && 1
	DrawDebugLine(GetWorld(), RootComponent->GetComponentLocation(), TargetPawn->GetActorLocation(), FColor::Red,
		false, 0.1f, 0, 4.f);
	
#endif
	APortalCharacter* TargetCharacter = Cast<APortalCharacter>(TargetPawn);

	if (!IsValid(TargetCharacter))
	{
		return;
	}
	
	TargetCharacter->DamageCharacter(TurretDamage, InDeltaTime);
}

void AamsuTurret::DeactivateTurret()
{
	SetActorTickEnabled(false);
}



