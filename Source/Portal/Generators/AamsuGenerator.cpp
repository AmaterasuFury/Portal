// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuGenerator.h"
#include "Components/ArrowComponent.h"

AamsuGenerator::AamsuGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshBridgeGenerator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bridge Generator Mesh"));
	MeshBridgeGenerator->SetupAttachment(RootComponent);

	Bridge = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bridge Mesh"));
	Bridge->SetupAttachment(RootComponent);

	InitialBridgeTransform = CreateDefaultSubobject<UArrowComponent>(TEXT("Initial Bridge Transform"));
	InitialBridgeTransform->SetupAttachment(RootComponent);
}

void AamsuGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SpawnBridge();
}


void AamsuGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void AamsuGenerator::SpawnBridge()
{
	if (!ensure(IsValid(Bridge) && IsValid(InitialBridgeTransform)))
	{
		return;
	}
	
	
	FHitResult HitResult;
	FCollisionQueryParams FCollisionQueryParams;
	FCollisionQueryParams.AddIgnoredActor(this);
	const FVector CheckDestination = InitialBridgeTransform->GetComponentLocation() + InitialBridgeTransform->GetComponentRotation().Vector() * MaxBridgeLength;
	
	GetWorld()->LineTraceSingleByChannel(HitResult, InitialBridgeTransform->GetComponentLocation(), CheckDestination,
		ECollisionChannel::ECC_Visibility, FCollisionQueryParams);
	
	const float BridgeLength = HitResult.Distance;

	if (BridgeLength < 0.1f)
	{
		return;
	}
	FVector Scale = FVector::OneVector;
	Scale.Y = BridgeLength/ BridgePartLength;
	
	const FVector Location = InitialBridgeTransform->GetComponentLocation() + InitialBridgeTransform->GetComponentRotation().Vector() * (BridgeLength / 2.f);
	
	Bridge->SetWorldScale3D(Scale);
	Bridge->SetWorldLocation(Location);
}


