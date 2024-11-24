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
	APlayerController* PlayerController = GetOwner<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}
	
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;
	GetOwner<APlayerController>()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	
	FVector HoldLocation = ViewLocation + ViewRotation.Vector() * HoldDistance;
	
	// TODO find distance and move this object to it with the speed based on the curve
}

