// Fill out your copyright notice in the Description page of Project Settings.


#include "Bridge.h"


ABridge::ABridge()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshComponentBridgeGenerator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bridge Generator Mesh"));
	MeshComponentBridgeGenerator->SetupAttachment(RootComponent);

	MeshFirstBridgePart
}


void ABridge::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABridge::SpawnBridge(TObjectPtr<UStaticMeshComponent> OriginalMesh)
{
	if (!IsValid(OriginalMesh))
	{
		return;
	}
	
}

