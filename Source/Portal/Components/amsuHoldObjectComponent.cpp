// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuHoldObjectComponent.h"
#include "Portal/CodeHelpers/amsuGetHelper.h"

DEFINE_LOG_CATEGORY(LogHoldObjectComponent)

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

void UamsuHoldObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PickUpAndCarry(DeltaTime);
}

void UamsuHoldObjectComponent::PickUpStart(AActor* InInteractedActor)
{
	if (bAlreadyHolding)
	{
		PickUpEnd();
		return;
	}
	InteractingActor = InInteractedActor;
	bAlreadyHolding = true;
	SetComponentTickEnabled(true);
	
	UE_LOG(LogHoldObjectComponent, Log, TEXT("Uamsu Hold Object Component::PickUpStart() Called"));
}

void UamsuHoldObjectComponent::PickUpEnd() 
{
	InteractingActor = nullptr;
	bAlreadyHolding = false;
	SetComponentTickEnabled(false);

	UE_LOG(LogHoldObjectComponent, Log, TEXT("Uamsu Hold Object Component::PickUpEnd() Called"));
}

void UamsuHoldObjectComponent::PickUpAndCarry(float InDeltaTime)   
{
	
	const APlayerController* PlayerController = amsuGetHelper::GetPlayerController(InteractingActor); 
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
//
	//FRotator RotationDelta = FRotator::ZeroRotator;
	//
	//GetOwner()->GetRootComponent()->AddLocalRotation(RotationDelta);
	
	const FVector TraceEnd = ViewLocation + ViewRotation.Vector() * HoldDistance;
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(InteractingActor);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceEnd, ECC_Visibility, CollisionQueryParams);

	FTimerManager& Manager = GetWorld()->GetTimerManager();
	
	if (HitResult.GetActor() != GetOwner() && !Manager.IsTimerActive(TimerTillDrop))
	{
		Manager.SetTimer(TimerTillDrop, this, &UamsuHoldObjectComponent::PickUpEnd, ObjectReleaseTime, false);
	}

	if (HitResult.GetActor() == GetOwner())
	{
		Manager.ClearTimer(TimerTillDrop);
	}
	

// todo	Sweep that we can move
// todo Follow the character rotation	
//	First task:
// 
	
}
