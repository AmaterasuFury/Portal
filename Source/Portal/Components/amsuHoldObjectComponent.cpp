// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuHoldObjectComponent.h"


// Sets default values for this component's properties
UamsuHoldObjectComponent::UamsuHoldObjectComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
}


// Called when the game starts
void UamsuHoldObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UamsuHoldObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UamsuHoldObjectComponent::PickUpAndCarry(AActor* InteractingActor)
{
	APlayerController* PlayerController = Cast<APlayerController>(InteractingActor);
	if (!(IsValid(PlayerController) && IsValid(GetOwner())))
	{
		return;
	} // todo think if this is a good way to check if the character is valid.
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;
	GetOwner<APlayerController>()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	
	const FVector HoldLocation = ViewLocation + ViewRotation.Vector() * HoldDistance;
	const FVector ObjectLocation = GetOwner()->GetActorLocation();
	
	FVector HoldDirection = (HoldLocation - ObjectLocation).GetSafeNormal();
	float CurrentDistance = (HoldLocation - ObjectLocation).Size();

	float MovementDistance = FMath::Clamp<float>(MovementSpeed * GetOwner()->GetWorld()->DeltaTimeSeconds, 0.f, CurrentDistance);
	FVector Movement = HoldDirection * MovementDistance;
	GetOwner()->GetRootComponent()->AddRelativeLocation(Movement);	

	
	
// todo	Drop if has blocking hit by LineTrace
// todo	Sweep that we can move
// todo Folow the character rotation	
// todo (later u can try to base it on the curve)
//	First task:
// 

	
}

