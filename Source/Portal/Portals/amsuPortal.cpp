// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portals/amsuPortal.h"

#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponentCube.h"


// Sets default values
AamsuPortal::AamsuPortal()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshComponentActivePortal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Active Portal Mesh"));
	MeshComponentActivePortal->SetupAttachment(RootComponent);

	MeshComponentInactivePortal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Inactive Portal Mesh"));
	MeshComponentInactivePortal->SetupAttachment(RootComponent);
		
	BoxOverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Overlap Comnponent"));
	BoxOverlapComponent->SetupAttachment(RootComponent);
	// TODO fix the BoxComponent overlap (in BP) to overelap with the portal channels only

	CaptureComponentCube = CreateDefaultSubobject<USceneCaptureComponentCube>(TEXT("Capture Component"));
	CaptureComponentCube->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AamsuPortal::BeginPlay()
{
	Super::BeginPlay();

	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	BoxOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);

	//** Sets the Portals invisible till they are being spawned by the portalgun */
	MakePortalVisible(false);
}

void AamsuPortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && !OtherActor->IsA<AamsuPortal>())
	{
		Teleport(OtherActor);
	}
}

void AamsuPortal::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

bool AamsuPortal::IsPortalVisible() const
{
	return bIsVisible;
}


void AamsuPortal::Teleport(AActor* InteractedActor) const
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

void AamsuPortal::MakePortalVisible(bool bMakeVisible) //t
{
	if (bIsVisible == bMakeVisible)
	{
		return;
	}

	ON_SCOPE_EXIT
	{
		OnPortalStateChange.Broadcast(bMakeVisible);	
	};
	
	bIsVisible = bMakeVisible;

	SetActorHiddenInGame(!bMakeVisible);  
	
	SetActorEnableCollision(bMakeVisible);

	SetActorTickEnabled(bMakeVisible);	

	// do 
	//MeshComponentActivePortal->SetMaterial(0, bMakeVisible ?  ActivePortalMaterial : InactivePortalMaterial);
	
	if (!IsValid(AnotherPortal))
	{
		return;
	}
	
	if (!bMakeVisible && AnotherPortal->IsPortalVisible())
	{
		AnotherPortal->MeshComponentActivePortal->SetHiddenInGame(true);
		AnotherPortal->MeshComponentInactivePortal->SetHiddenInGame(false);
		
		return;
	}
	
	
	if (AnotherPortal->IsPortalVisible())
	{
		MeshComponentActivePortal->SetHiddenInGame(false);
		MeshComponentInactivePortal->SetHiddenInGame(true);
		AnotherPortal->MeshComponentActivePortal->SetHiddenInGame(false);
		AnotherPortal->MeshComponentInactivePortal->SetHiddenInGame(true);
	}
	else
	{
		MeshComponentActivePortal->SetHiddenInGame(true);
		MeshComponentInactivePortal->SetHiddenInGame(false);
		AnotherPortal->MeshComponentActivePortal->SetHiddenInGame(true);
		AnotherPortal->MeshComponentInactivePortal->SetHiddenInGame(false);
	}
}


void AamsuPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
 