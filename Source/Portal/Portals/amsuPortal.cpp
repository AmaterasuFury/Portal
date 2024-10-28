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

	SetPortalVisibility(false);
}

// Called when the game starts or when spawned
void AamsuPortal::BeginPlay()
{ // todo By default make them hidden (use the function that u are going to create as addon to the bIsActive
	Super::BeginPlay();

	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	BoxOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
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

void AamsuPortal::Teleport(AActor* InteractedActor)
{
	//FVector OutTeleportDistance(0.f, 200.f, 0.f);

	if (!ensure(IsValid(AnotherPortal)))
	{
		return;
	}
	
  	FVector TeleportLocation = AnotherPortal->GetActorLocation() * AnotherPortal->GetActorForwardVector();
	InteractedActor->SetActorLocation( TeleportLocation);
	// Todo direction
}

void AamsuPortal::SetPortalVisibility(bool MakeVisible)
{
	SetActorHiddenInGame(!MakeVisible);

	SetActorEnableCollision(MakeVisible);

	SetActorTickEnabled(MakeVisible);
}


void AamsuPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
