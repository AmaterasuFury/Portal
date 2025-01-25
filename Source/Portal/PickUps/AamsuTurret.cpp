// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuTurret.h"
#include "Engine/OverlapResult.h"


AamsuTurret::AamsuTurret()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AamsuTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** Notice: the function checks the actors by their location, if the actor is big, the function might not detect it.
 * If u want to make this function more accurate u would have to implement a logic to additionally trace a linetrace to actors angle, but it would cost you more of the performance, of course */
TArray<AActor*> AamsuTurret::FOVActorsCheck(float InCheckDistance, ECollisionChannel InCollisionChannel) const
{ 
	const TArray<AActor*> AllActorsInRadius = GetAllActorsInRadius(InCheckDistance, InCollisionChannel);
	TArray<AActor*> ActorsInFOV;

	const FVector OwnerPosition = GetOwner()->GetActorLocation();
	const FVector NormalizedOwnerForward = GetOwner()->GetActorForwardVector();
	
	TArray<FVector> NormalizedDirectionToTargets;
	
	for (const auto ActorInRadius : AllActorsInRadius)
	{
		if (IsValid(ActorInRadius))
		{
			const FVector NormalizedDirectionToTarget = (ActorInRadius->GetActorLocation() - OwnerPosition).GetSafeNormal();

			const float DotProduct = FVector::DotProduct(NormalizedOwnerForward, NormalizedDirectionToTarget);
			
			const float Threshold = FMath::Cos(FMath::DegreesToRadians(FOVAngle / 2));
			
			if (DotProduct >= Threshold && !IsActorCovered(OwnerPosition, ActorInRadius)) 
			{
				ActorsInFOV.Add(ActorInRadius);
			}
		}
	}
	
	return ActorsInFOV;
}

TArray<AActor*> AamsuTurret::GetAllActorsInRadius(float InCheckDistance, ECollisionChannel InCollisionChannel) const
{
	TArray<AActor*> FoundActors;
	TArray<FOverlapResult> OutOverlaps;
	FCollisionQueryParams FCollisionQueryParams;
	
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner());
	
	FCollisionQueryParams.AddIgnoredActors(IgnoredActors);
	
	GetWorld()->OverlapMultiByChannel(OutOverlaps, GetOwner()->GetActorLocation(), FQuat::Identity, InCollisionChannel,
		FCollisionShape::MakeSphere(InCheckDistance), FCollisionQueryParams);
	

	
	
	for (const FOverlapResult& OutOverlap : OutOverlaps)
	{
		if (!IsValid(OutOverlap.GetActor()))
		{
			continue;
		}
		FoundActors.Add(OutOverlap.GetActor());
	}
	
	return FoundActors;
}

bool AamsuTurret::IsActorCovered(FVector OwnerPosition, AActor* TargetActor, ECollisionChannel InCollisionChannel) const
{
	FHitResult HitResult;
	FCollisionQueryParams FCollisionQueryParams;

	FCollisionQueryParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, OwnerPosition, TargetActor->GetActorLocation(), InCollisionChannel, FCollisionQueryParams );
	
	return HitResult.GetActor() != TargetActor;
}
