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

	PickUpAndCarry();
}

void UamsuHoldObjectComponent::PickUpStart(AActor* InInteractedActor)
{
	SetComponentTickEnabled(true);
	InteractingActor = InInteractedActor;
}

void UamsuHoldObjectComponent::PickUpEnd()
{
	SetComponentTickEnabled(false);
	InteractingActor = nullptr;
}

void UamsuHoldObjectComponent::PickUpAndCarry() const // todo pass deltatime as argument and use it in the tick  
{ 
	const APlayerController* PlayerController = Cast<APlayerController>(InteractingActor);
	if (!(IsValid(PlayerController) && IsValid(GetOwner())))
	{
		return;
	} 
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;
	
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	
	const FVector HoldLocation = ViewLocation + ViewRotation.Vector() * HoldDistance;
	const FVector ObjectLocation = GetOwner()->GetActorLocation();
	
	const FVector HoldDirection = (HoldLocation - ObjectLocation).GetSafeNormal();
	const float CurrentDistance = (HoldLocation - ObjectLocation).Size();
	
	if (!ensure(IsValid(GetWorld())))
	{
		return;
	}
	const float MovementDistance = FMath::Clamp<float>(MovementSpeed * GetOwner()->GetWorld()->DeltaTimeSeconds, 0.f, CurrentDistance);
	const FVector Delta = HoldDirection * MovementDistance;

	if (!ensure(IsValid(GetOwner()->GetRootComponent())))
	{
		return;
	}
	GetOwner()->GetRootComponent()->AddRelativeLocation(Delta);	

	
	
// todo	Drop if has blocking hit by LineTrace
// todo	Sweep that we can move
// todo Follow the character rotation	
// todo (later u can try to base it on the curve)
//	First task:
// 
	
}

