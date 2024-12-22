// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuIntreactionDetectComponent.h"

#include "Portal/amsuInteractable.h"
#include "Portal/MyCollisionChannels.h"

UamsuInteractionDetectComponent::UamsuInteractionDetectComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UamsuInteractionDetectComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

TScriptInterface<IamsuInteractable> UamsuInteractionDetectComponent::GetAimedInteractable() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!ensure(IsValid(OwnerPawn)))
	{
		return nullptr;
	}
	
	const APlayerController* OwningController = Cast<APlayerController>(OwnerPawn->GetController());

	if (!IsValid(OwningController))
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
	
	OwningController->GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceEnd, Portal::ECC_Interaction, CollisionQueryParams);

	AActor* HitActor = HitResult.GetActor();
	
	return IsValid(HitActor) && HitActor->Implements<UamsuInteractable>() ? HitActor : nullptr;
}


