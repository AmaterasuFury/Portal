// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portals/amsuPortal.h"

#include "Components/BoxComponent.h"


// Sets default values
AamsuPortal::AamsuPortal()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshComponentPortal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Mesh"));
	MeshComponentPortal->SetupAttachment(RootComponent);

	BoxOverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Overlap Comnponent"));
	BoxOverlapComponent->SetupAttachment(RootComponent);

	// TODO fix the BoxComponent overlap (in BP) to overelap with the portal channels only
}

// Called when the game starts or when spawned
void AamsuPortal::BeginPlay()
{
	Super::BeginPlay();

	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	BoxOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);

	SetPortalVisibility(false);
}

void AamsuPortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		Teleport(OtherActor);
	}
}

void AamsuPortal::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// TODO upgrade this function to hide the portal in the world based on the bool that it gets
void AamsuPortal::ActivatePortal(bool bActivate)
{
	if (bIsActive == bActivate)
	{
		return;
	}

	bIsActive = bActivate;
	OnPortalStateChange.Broadcast(bActivate);
}

void AamsuPortal::Teleport(AActor* InteractedActor)
{
	if (!ensure(IsValid(AnotherPortal)))
	{
		return;
	}

	const double TeleportDistance = 200.0;
	
  	const FVector TeleportLocation = AnotherPortal->GetActorLocation() + AnotherPortal->GetActorForwardVector() * TeleportDistance;
	InteractedActor->SetActorLocation(TeleportLocation);

	const FVector NewDirection = AnotherPortal->GetActorForwardVector().Rotation().RotateVector(InteractedActor->GetActorForwardVector());
	
	const FRotator ResultRotation = NewDirection.Rotation();

	if (const APawn* Pawn = Cast<APawn>(InteractedActor); IsValid(Pawn))
	{
		if (const AController* Controller = Pawn->GetController(); IsValid(Controller))
		{
			Pawn->GetController()->SetControlRotation(ResultRotation);
			return;
		}
	}
	InteractedActor->SetActorRotation(ResultRotation);
}

void AamsuPortal::SetPortalVisibility(bool bMakeVisible)
{
	SetActorHiddenInGame(!bMakeVisible);

	SetActorEnableCollision(bMakeVisible);

	SetActorTickEnabled(bMakeVisible);
}


void AamsuPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
 