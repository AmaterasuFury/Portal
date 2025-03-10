// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portals/amsuPortal.h"

#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"


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

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture Component"));
	CaptureComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AamsuPortal::BeginPlay()
{
	Super::BeginPlay();

	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	BoxOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);

	ActivatePortal(false);
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

void AamsuPortal::OnPortalPlaced(bool bPlacePortal)
{
	if (bIsActive == bPlacePortal)
	{
		return;
	}
	
	ActivatePortal(bPlacePortal);

	bIsActive = bPlacePortal;
	OnPortalStateChange.Broadcast(bPlacePortal);
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

void AamsuPortal::ActivatePortal(bool bMakeVisible)
{
	SetActorHiddenInGame(!bMakeVisible);

	SetActorEnableCollision(bMakeVisible);

	SetActorTickEnabled(bMakeVisible);
}

void AamsuPortal::UpdateSceneCaptureRotation() const
{
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController)) return;
	
	const APawn* PawnCharacter = PlayerController->GetPawn();
	if (!IsValid(PawnCharacter)) return;
	
	// Get player camera location and rotation
	FVector PlayerLocation;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);

	// Compute relative vector from this portal to player
	FVector PortalToPlayer = PlayerLocation - GetActorLocation();

	// Reflect the vector across the portal's forward direction
	FVector ReflectedDirection = FMath::GetReflectionVector(PortalToPlayer.GetSafeNormal(), GetActorForwardVector());

	// Set the capture component rotation to the reflected view direction
	FRotator NewRotation = ReflectedDirection.Rotation();

	AnotherPortal->CaptureComponent->SetWorldRotation(NewRotation);
	AnotherPortal->CaptureComponent->CaptureScene();
}


void AamsuPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateSceneCaptureRotation();
}
 