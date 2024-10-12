// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuSlidingDoor.h"

#include "Components/BoxComponent.h"

// Sets default values
AamsuSlidingDoor::AamsuSlidingDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshComponentLeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door Mesh"));
	MeshComponentLeftDoor->SetupAttachment(RootComponent);

	MeshComponentRightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door Mesh"));
	MeshComponentRightDoor->SetupAttachment(RootComponent);

	BoxOverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Overlap Component"));
	BoxOverlapComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AamsuSlidingDoor::BeginPlay()
{
	Super::BeginPlay();

	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	BoxOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}

void AamsuSlidingDoor::OpenDoor(float InDeltaTime)
{
	if (bIsLocked)
	{
		return;
	}
	if (FMath::IsNearlyEqual(CurrentPosition, SlideRange))
	{
		return;
	}
	if (bDoorIsOpening)
	{
		TickDoorSlide(InDeltaTime, true);
	}
}

void AamsuSlidingDoor::Open()
{
	IsOpeningAndTickOn();
	
	FTimerHandle OpenTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(OpenTimerHandle, this, &ThisClass::Close, CloseInSeconds, false);
}

void AamsuSlidingDoor::IsOpeningAndTickOn()
{
	bDoorIsOpening = true;
	SetActorTickEnabled(true);
}

void AamsuSlidingDoor::Close()
{
	bDoorIsOpening = false;
}

void AamsuSlidingDoor::CloseDoor(float InDeltaTime)
{
	if (FMath::IsNearlyEqual(CurrentPosition, 0.0f))
	{
		SetActorTickEnabled(false);
		return;
	}
	if (!bDoorIsOpening)
	{
		TickDoorSlide(InDeltaTime, false);
	}
}

void AamsuSlidingDoor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IsOpeningAndTickOn();
}

void AamsuSlidingDoor::TickDoorSlide(float InDeltaTime, bool bDoorOpens)
{
	float DeltaPosition = InDeltaTime * OpenSpeed;
	const float OldPosition = CurrentPosition;
	float OldPlusDelta = 0.0f;

	if (bDoorOpens)
	{
		OldPlusDelta = OldPosition + DeltaPosition;
	}
	if (!bDoorOpens)
	{
		OldPlusDelta = OldPosition - DeltaPosition;
	}

	CurrentPosition = FMath::Clamp<float>(OldPlusDelta, 0.0f, SlideRange);

	DeltaPosition = CurrentPosition - OldPosition;
	
	const FVector DeltaVector (DeltaPosition, 0.0, 0.0);

	MeshComponentLeftDoor->AddRelativeLocation(-DeltaVector);
	MeshComponentRightDoor->AddRelativeLocation(DeltaVector);
}

void AamsuSlidingDoor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bDoorIsOpening = false;
}

// Called every frame
void AamsuSlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenDoor(DeltaTime);
	CloseDoor(DeltaTime);
}

