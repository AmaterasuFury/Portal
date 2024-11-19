// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuBridge.h"
#include "Components/ArrowComponent.h"

AamsuBridge::AamsuBridge()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshBridgeGenerator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bridge Generator Mesh"));
	MeshBridgeGenerator->SetupAttachment(RootComponent);

	BridgePart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("First Bridge Part Mesh"));
	BridgePart->SetupAttachment(RootComponent);

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
	if (!ensure(IsValid(BridgePart)))
	{
		return;
	}

	FTransform TargetTransform = InitialBridgeTransform->GetComponentTransform();
	TargetTransform.AddToTranslation(InitialBridgeTransform->GetForwardVector() * BridgePartLength / 2.0);

	BridgePart->SetVisibility(true);
	BridgePart->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	

	FHitResult HitResult;
	FCollisionQueryParams FCollisionQueryParams;
	FCollisionQueryParams.AddIgnoredActor(this);
	const FVector CheckDestination = InitialBridgeTransform->GetComponentLocation() + InitialBridgeTransform->GetComponentRotation().Vector() * MaxBridgeLength;
	
	GetWorld()->LineTraceSingleByChannel(HitResult, InitialBridgeTransform->GetComponentLocation(), CheckDestination,
		ECollisionChannel::ECC_Visibility, FCollisionQueryParams);
	
	const float FullPartsLength = HitResult.Distance - FMath::Fmod(HitResult.Distance, BridgePartLength);	
	
	for (float Dist = 0; Dist < FullPartsLength; Dist += BridgePartLength)
	{
		UStaticMeshComponent* NewPart = DuplicateObject<UStaticMeshComponent>(BridgePart, this);
		NewPart->RegisterComponent();
	 	
		NewPart->SetWorldTransform(TargetTransform);

		TargetTransform.AddToTranslation(InitialBridgeTransform->GetForwardVector() * BridgePartLength);

		BridgeParts.Add(NewPart);
	}

	// TODO Find a way to add the last part of the Bridge and to cut the mesh or find smth out
	
	BridgePart->SetVisibility(false);
	BridgePart->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AamsuBridge::DestroyBridge()
{
	for (UStaticMeshComponent* Part : BridgeParts)
	{
		Part->DestroyComponent();
	}
	BridgeParts.Empty();
}

