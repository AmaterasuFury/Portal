// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuBridge.h"
#include "Components/ArrowComponent.h"

AamsuBridge::AamsuBridge()
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

void AamsuBridge::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SpawnBridge();
}


void AamsuBridge::BeginPlay()
{
	Super::BeginPlay();
}

void AamsuBridge::SpawnBridge()
{
	if (!ensure(IsValid(Bridge)))
	{
		return;
	}
	
	
	FHitResult HitResult;
	FCollisionQueryParams FCollisionQueryParams;
	FCollisionQueryParams.AddIgnoredActor(this);
	const FVector CheckDestination = InitialBridgeTransform->GetComponentLocation() + InitialBridgeTransform->GetComponentRotation().Vector() * MaxBridgeLength;
	
	GetWorld()->LineTraceSingleByChannel(HitResult, InitialBridgeTransform->GetComponentLocation(), CheckDestination,
		ECollisionChannel::ECC_Visibility, FCollisionQueryParams);
	
	float BridgeLength = HitResult.Distance;

	const float BridgeWidth = Bridge->GetRelativeScale3D().X;
	const float BridgeHeight = Bridge->GetRelativeScale3D().Z;
	
	if (BridgeLength > 0.1f)
	{
		Bridge->SetRelativeScale3D(FVector(BridgeWidth, BridgeLength,BridgeHeight));

		/* FVector Scale = Bridge->GetRelativeScale3D();
			Scale.Y = NewLength / DefaultLength;
			Bridge->SetRelativeScale3D(Scale);*/
	} 
}


