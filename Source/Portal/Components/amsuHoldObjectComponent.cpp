// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuHoldObjectComponent.h"


// Sets default values for this component's properties
UamsuHoldObjectComponent::UamsuHoldObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
}


// Called when the game starts
void UamsuHoldObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UamsuHoldObjectComponent::CancelTimer()
{
	if (TraceTimerSet)
	{
		TimerTillDrop.Invalidate();
	}
}


void UamsuHoldObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PickUpAndCarry(DeltaTime);
}

void UamsuHoldObjectComponent::PickUpStart(AActor* InInteractedActor)
{
	InteractingActor = InInteractedActor;
	SetComponentTickEnabled(true);
}

void UamsuHoldObjectComponent::PickUpEnd() 
{
	InteractingActor = nullptr;
	SetComponentTickEnabled(false);
}
PRAGMA_DISABLE_OPTIMIZATION
void UamsuHoldObjectComponent::PickUpAndCarry(float InDeltaTime)   
{
	const APawn* InteractingPawn = Cast<APawn>(InteractingActor);
	const APlayerController* PlayerController = Cast<APlayerController>(InteractingPawn->GetController());
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
	const float MovementDistance = FMath::Clamp<float>(MovementSpeed * InDeltaTime, 0.f, CurrentDistance);
	const FVector Delta = HoldDirection * MovementDistance;

	if (!ensure(IsValid(GetOwner()->GetRootComponent())))
	{
		return;
	}
	GetOwner()->GetRootComponent()->AddRelativeLocation(Delta, true);	

	
	const FVector TraceEnd = ViewLocation + ViewRotation.Vector() * HoldDistance;
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(InteractingActor);
	// todo delete the Playcontroller, just use GetWorld()
	PlayerController->GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceEnd, ECC_Visibility, CollisionQueryParams);

	if (HitResult.GetActor() != GetOwner() && !TraceTimerSet)
	{
		TraceTimerSet = true;
		//PickUpEnd();
	}
	
	if (TraceTimerSet && !TimerTillDrop.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerTillDrop, this, &UamsuHoldObjectComponent::PickUpEnd, ObjectReleaseTime, false);
	}
	
	
// todo	Drop if has blocking hit by LineTrace
// todo	Sweep that we can move
// todo Follow the character rotation	
//	First task:
// 
	
}

PRAGMA_ENABLE_OPTIMIZATION