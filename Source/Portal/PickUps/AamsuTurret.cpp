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

	TArray<APawn*> FOVCharacters = FOVCharactersCheck(CheckDistance);

	OnActiveMode(bIsInActiveRadius);
}
PRAGMA_DISABLE_OPTIMIZATION
void AamsuTurret::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(true);

	PlayerControllersInWorld = amsuGetHelper::GetAllPlayerControllers(GetWorld());
}

// todo fix description
/** If u want to make this function more accurate u would have to implement a logic to additionally trace a linetrace to actors angle, but it would cost you more of the performance, of course */
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

	TArray<APawn*> PlayerControllersInFOV;
	
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
				PlayerControllersInFOV.Add(PlayerPawn);
			}
		}
	}
	
	return PlayerControllersInFOV;
}


bool AamsuTurret::IsActorCovered(const FVector& OwnerPosition, AActor* TargetActor, ECollisionChannel InCollisionChannel) const
{
	FHitResult HitResult;
	FCollisionQueryParams FCollisionQueryParams;
	
	FCollisionQueryParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, OwnerPosition, TargetActor->GetActorLocation(), InCollisionChannel, FCollisionQueryParams);

#if ENABLE_DRAW_DEBUG && 1
	if (HitResult.GetActor() == TargetActor)
	{
		DrawDebugLine(GetWorld(), OwnerPosition, TargetActor->GetActorLocation(), FColor::Green,
		false, 0.1f, 0, 4.f);
	}
#endif
	
	return HitResult.GetActor() != TargetActor;
}
PRAGMA_ENABLE_OPTIMIZATION
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

void AamsuTurret::Shoot(APawn* Pawn)
{
	// TODO first shoot a debuglinetrace
}

