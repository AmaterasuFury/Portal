// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portals/amsuPortal.h"

#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponentCube.h"
#include "Engine/OverlapResult.h"



DEFINE_LOG_CATEGORY(amsuPortal)

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
	PortalAttachedActors.Reserve(15);
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

void AamsuPortal::IgnoreOverlappedActor(AActor* OverlappedActor, bool bIgnore) const
{
	if (!IsValid(OverlappedActor))
	{
		return;
	}
	for (AActor* PortalIsPlacedOnActor : PortalAttachedActors)
	{
		UPrimitiveComponent* OverlappedPrimitiveRoot = Cast<UPrimitiveComponent>(PortalIsPlacedOnActor->GetRootComponent());

		if (!ensure(IsValid(OverlappedPrimitiveRoot)))
		{
			UE_LOG(amsuPortal, Log, TEXT("The actors portal is placed on should have root component that is derived from UPrimitiveComponent"));
			return;
		}
		//OverlappedPrimitiveRoot->IgnoreActorWhenMoving(OverlappedActor, bIgnore);

		TInlineComponentArray<UPrimitiveComponent*> OverlappedPrimitiveComponents(OverlappedActor);
		for (UPrimitiveComponent* OverlappedPrimitiveComponent : OverlappedPrimitiveComponents)
		{
			if (!ensure(IsValid(OverlappedPrimitiveComponent)))
			{
				continue;
			}
			OverlappedPrimitiveComponent->IgnoreActorWhenMoving(PortalIsPlacedOnActor, bIgnore);
		}
	}
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

		if (!bMakeVisible)
		{
			PortalAttachedActors.Reset();	
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

void AamsuPortal::AssignPortalAttachedActors(const FVector& PortalSpawnLocation, const FRotator& PortalSpawnRotation)
{
	PortalAttachedActors.Reset();
	
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
		PortalAttachedActors.Add(Overlap.GetActor());
	}
}

void AamsuPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
 