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


void AamsuBridge::BeginPlay()
{
	Super::BeginPlay();

	SpawnBridge();
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
	
	// Use a trace to find out how much you should spawn
	for (double Dist = 0; Dist < MaxBridgeLength; Dist += BridgePartLength)
	{
		UStaticMeshComponent* NewPart = DuplicateObject<UStaticMeshComponent>(BridgePart, this);
		NewPart->RegisterComponent();
		
		NewPart->SetWorldTransform(TargetTransform);

		TargetTransform.AddToTranslation(InitialBridgeTransform->GetForwardVector() * BridgePartLength);

		BridgeParts.Add(NewPart);
	}

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

