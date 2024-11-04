// Fill out your copyright notice in the Description page of Project Settings.


#include "Bridge.h"

#include "Components/ArrowComponent.h"


ABridge::ABridge()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshComponentBridgeGenerator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bridge Generator Mesh"));
	MeshComponentBridgeGenerator->SetupAttachment(RootComponent);

	MeshFirstBridgePart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("First Bridge Part Mesh"));
	MeshFirstBridgePart->SetupAttachment(RootComponent);

	InitialBridgeTransform = CreateDefaultSubobject<UArrowComponent>(TEXT("Initial Bridge Transform"));
	InitialBridgeTransform->SetupAttachment(RootComponent);
}


void ABridge::BeginPlay()
{
	Super::BeginPlay();

	SpawnBridge();
	
}

void ABridge::SpawnBridge()
{
	if (!ensure(IsValid(MeshFirstBridgePart)))
	{
		return;
	}

	DestroyBridge();
	
	FTransform TargetTransform = InitialBridgeTransform->GetComponentTransform();
	TargetTransform.AddToTranslation(InitialBridgeTransform->GetForwardVector() * BridgePartLength / 2.0);

	MeshFirstBridgePart->SetVisibility(true);
	MeshFirstBridgePart->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	for (double Dist = 0; Dist < MaxBridgeLength; Dist += BridgePartLength)
	{
		UStaticMeshComponent* NewPart = DuplicateObject<UStaticMeshComponent>(MeshFirstBridgePart, this);
		NewPart->RegisterComponent();

		NewPart->SetWorldTransform(TargetTransform);

		TargetTransform.AddToTranslation(InitialBridgeTransform->GetForwardVector() * BridgePartLength);

		BridgeParts.Add(NewPart);
	}

	MeshFirstBridgePart->SetVisibility(false);
	MeshFirstBridgePart->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABridge::DestroyBridge()
{
	for (UStaticMeshComponent* Part : BridgeParts)
	{
		Part->DestroyComponent();
	}
	BridgeParts.Empty();
}
