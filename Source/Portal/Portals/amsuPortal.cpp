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

	//** Sets the portal size*/
	FVector Origin = GetActorLocation();
	FVector BoxExtent = FVector::ZeroVector;
	MeshComponentPortal->GetLocalBounds(Origin, BoxExtent);
	PortalsHalfWidth = BoxExtent.X;
	PortalsHalfHeight = BoxExtent.Z;

	//** Reserves memory for the array of the actors the portal is placed on*/
	ActorsPortalIsPlacedOn.Reserve(15);
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
	
	IgnoreOverlappedActor(InteractedActor, true);
}

void AamsuPortal::TeleportEnd(AActor* InteractingActor) const
{
	if (!ensure(IsValid(AnotherPortal)))
	{
		return;
	}
	
	IgnoreOverlappedActor(InteractingActor, false);
}
//PRAGMA_DISABLE_OPTIMIZATION
void AamsuPortal::IgnoreOverlappedActor(AActor* OverlappedActor, bool bIgnore) const
{
	if (!IsValid(OverlappedActor))
	{
		return;
	}
	for (AActor* PortalIsPlacedOnActor : ActorsPortalIsPlacedOn)
	{
		UPrimitiveComponent* OverlappedPrimitiveRoot = Cast<UPrimitiveComponent>(PortalIsPlacedOnActor->GetRootComponent());

		if (!ensure(IsValid(OverlappedPrimitiveRoot)))
		{
			UE_LOG(LogTemp, Log, TEXT("The actors portal is placed on should have root component that is derived from UPrimitiveComponent"));
			return;
		}
		OverlappedPrimitiveRoot->IgnoreActorWhenMoving(OverlappedActor, bIgnore);
	}
}
//PRAGMA_ENABLE_OPTIMIZATION
void AamsuPortal::MakePortalVisible(bool bMakeVisible) 
{
	if (bIsVisible == bMakeVisible)
	{
		return;
	}

	ON_SCOPE_EXIT
	{
		OnPortalStateChange.Broadcast(bMakeVisible);

		if (!bMakeVisible)
		{
			ActorsPortalIsPlacedOn.Reset();	
		}
	};
	
	bIsVisible = bMakeVisible;

	SetActorHiddenInGame(!bMakeVisible);  
	
	SetActorEnableCollision(bMakeVisible);

	// will set the tick on only when overlaps
	//SetActorTickEnabled(bMakeVisible);	
	
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

void AamsuPortal::SetPortalIsPlacedOn(const FVector& PortalSpawnLocation, const FRotator& PortalSpawnRotation)
{
	ActorsPortalIsPlacedOn.Reset();
	
	/** Check what actors the portal is placed on */
	TArray<FOverlapResult> Overlaps;

	constexpr float BoxCheckDepth = -5.0f;
	const FVector BoxCheckLocation = PortalSpawnLocation + PortalSpawnRotation.Vector() * BoxCheckDepth;

	constexpr float BoxExtendDepth = 5.0f;
	const FVector BoxExtent = FVector(BoxExtendDepth, PortalsHalfWidth, PortalsHalfHeight);
	
	const FQuat Rotation = PortalSpawnRotation.Quaternion();
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	GetWorld()->OverlapMultiByChannel(Overlaps, BoxCheckLocation, Rotation, ECC_WorldStatic,
		FCollisionShape::MakeBox(BoxExtent), QueryParams);
	//** Draw box debug if you need*/
	//DrawDebugBox(GetWorld(), BoxCheckLocation, BoxExtent, Rotation, FColor::Green, false, 100.0f);
	
	for (const FOverlapResult& Overlap : Overlaps)
	{
		ActorsPortalIsPlacedOn.Add(Overlap.GetActor());
	}
}

void AamsuPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
 