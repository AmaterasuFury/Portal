// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuIntreactionDetectComponent.h"

#include "Portal/amsuInteractable.h"


UamsuIntreactionDetectComponent::UamsuIntreactionDetectComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UamsuIntreactionDetectComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

AActor* UamsuIntreactionDetectComponent::DetectInteractable()
{
	const APlayerController* OwningController = Cast<APlayerController>(GetOwner());
	if (!ensure(IsValid(OwningController)))
	{
		return nullptr;
	}

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	OwningController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	const FVector TraceEnd = ViewLocation + ViewRotation.Vector() * CheckDistance;
	
	
	OwningController->GetWorld()->LineTraceSingleByChannel(HitResult,ViewLocation, TraceEnd, ECC_Visibility, CollisionQueryParams);

	AActor* HitActor = HitResult.GetActor();
	if (HitActor)
	{
		 return HitActor->Implements<UamsuInteractable>() ? HitActor : nullptr;
	}
	else
	{
		return nullptr;
	}
}


