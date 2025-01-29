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

	TArray<APlayerController*> FOVCharacters = FOVCharactersCheck(CheckDistance);

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
TArray<APlayerController*> AamsuTurret::FOVCharactersCheck(float InCheckDistance) const
{
	TArray<APlayerController*> PlayerControllersInRadius;
	if (PlayerControllersInWorld.IsEmpty())
	{
		return PlayerControllersInWorld;
	}
	
	if (!IsValid(RootComponent))
	{
		UE_LOG(amsuTurret, Log, TEXT("WARNING, the RootComponet of the amsuTurret is not valid"))
	}
	//const AActor* ThisActor = this;
	const FVector TurretLocation = RootComponent->GetComponentLocation();;
	
	
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
	
	const FVector NormalizedOwnerForward = RootComponent->GetForwardVector();
	TArray<FVector> NormalizedDirectionToTargets;
	
	for (APlayerController* PlayerController: PlayerControllersInRadius)
	{
		APawn* CharacterPawn = PlayerController->GetPawn();
		if (IsValid(CharacterPawn))
		{
			const FVector NormalizedDirectionToTarget = (CharacterPawn->GetActorLocation() - TurretLocation).GetSafeNormal();

			const float DotProduct = FVector::DotProduct(NormalizedOwnerForward, NormalizedDirectionToTarget);
			
			const float Threshold = FMath::Cos(FMath::DegreesToRadians(FOVAngle / 2));
			
			bool bTempBool = IsCharacterCovered(TurretLocation, CharacterPawn, ECC_Pawn);
			
			if (DotProduct >= Threshold && !bTempBool) 
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

	//FCollisionQueryParams.AddIgnoredActor(this);
	FCollisionQueryParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, OwnerPosition, TargetActor->GetActorLocation(), InCollisionChannel, FCollisionQueryParams);

	FVector Temp1TargetACtorVector = TargetActor->GetActorLocation();
#if ENABLE_DRAW_DEBUG && 1
	FVector TempTargetACtorVector = TargetActor->GetActorLocation();
	DrawDebugLine(GetWorld(), OwnerPosition, TempTargetACtorVector, FColor::Green,
		false, 0.1f, 0, 4.f);
#endif
	
	
	AActor* TempActor = HitResult.GetActor();
	
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

