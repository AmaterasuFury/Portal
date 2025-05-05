// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portals/amsuPortal.h"

#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponentCube.h"
#include "Engine/OverlapResult.h"



// Sets default values
AamsuPortal::AamsuPortal()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshComponentPortal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Mesh"));
	MeshComponentPortal->SetupAttachment(RootComponent);

	MeshComponentPortalFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Frame Mesh"));
	MeshComponentPortalFrame->SetupAttachment(RootComponent);
		
	BoxOverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Overlap Comnponent"));
	BoxOverlapComponent->SetupAttachment(RootComponent);
	// TODO fix the BoxComponent overlap (in BP) to overelap with the portal channels only

	CaptureComponentCube = CreateDefaultSubobject<USceneCaptureComponentCube>(TEXT("Capture Component"));
	CaptureComponentCube->SetupAttachment(RootComponent);

	InvisibleBackFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Invisible Back Frame"));
	InvisibleBackFrame->SetupAttachment(RootComponent);
	InvisibleBackFrame->SetHiddenInGame(true);
	// TODO set this mesh collision to some portalFrame channel so it will only colide when overlapp with portals(create it)
}

// Called when the game starts or when spawned
void AamsuPortal::BeginPlay()
{
	Super::BeginPlay();

	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	BoxOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);

	//** Sets the Portals invisible till they are being spawned by the portalgun */
	MakePortalVisible(false);

	FVector Origin = GetActorLocation();
	FVector BoxExtent = FVector::ZeroVector;
	MeshComponentPortal->GetLocalBounds(Origin, BoxExtent);
	
	PortalsHalfWidth = BoxExtent.X;
	PortalsHalfHeight = BoxExtent.Z;

	PortalIsPlacedOn.Reserve(10);
}

void AamsuPortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && !OtherActor->IsA<AamsuPortal>())
	{
		//Teleport(OtherActor);
		TeleportStart(OtherActor);
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

void AamsuPortal::TeleportStart(AActor* InteractedActor) const
{
	if (!ensure(IsValid(AnotherPortal)))
	{
		return;
	}
	
	// TODO 
	// use the PortalIsPlacedOn to ignore overalpp with the walls its on  and the other portal
	
}

void AamsuPortal::TeleportEnd(AActor* InteractingActor) const
{
}

void AamsuPortal::SetCollisionOff() const
{
}

void AamsuPortal::MakePortalVisible(bool bMakeVisible) 
{
	if (bIsVisible == bMakeVisible)
	{
		return;
	}

	ON_SCOPE_EXIT
	{
		OnPortalStateChange.Broadcast(bMakeVisible);
		SetPortalIsPlacedOn();
	};
	
	bIsVisible = bMakeVisible;

	SetActorHiddenInGame(!bMakeVisible);  
	
	SetActorEnableCollision(bMakeVisible);

	SetActorTickEnabled(bMakeVisible);	
	
	if (!IsValid(AnotherPortal))
	{
		return;
	}
	
	if (!bMakeVisible && AnotherPortal->IsPortalVisible())
	{
		AnotherPortal->MeshComponentPortal->SetMaterial(0, InactivePortalMaterial);
		return;
	}
	
	MeshComponentPortal->SetMaterial(0, AnotherPortal->IsPortalVisible() ? ActivePortalMaterial : InactivePortalMaterial);
	AnotherPortal->MeshComponentPortal->SetMaterial(0, AnotherPortal->IsPortalVisible() ? ActivePortalMaterial : InactivePortalMaterial);
}

void AamsuPortal::SetPortalIsPlacedOn()
{
	/** Check what actors the portal is placed on */
	TArray<FOverlapResult> Overlaps;
	
	const FVector BoxCenter = GetActorLocation() + GetActorForwardVector() * -1.0f;
	constexpr float HalfDepth = 5.0f;
	const FVector BoxExtent = FVector(HalfDepth, PortalsHalfWidth, PortalsHalfHeight);
	
	const FQuat Rotation = GetActorRotation().Quaternion();
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	GetWorld()->OverlapMultiByChannel(Overlaps, BoxCenter, Rotation, ECC_WorldStatic,
		FCollisionShape::MakeBox(BoxExtent), QueryParams);
	//** Draw box debug if you need*/
	//DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, Rotation, FColor::Green, false, 100.0f);
	
	PortalIsPlacedOn.Empty();
	
	for (const FOverlapResult& Overlap : Overlaps)
	{
		PortalIsPlacedOn.Add(Overlap.GetActor());
	}
}

void AamsuPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
 