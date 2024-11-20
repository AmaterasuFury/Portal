// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuPickUpObjectsBase.h"


AamsuPickUpObjectsBase::AamsuPickUpObjectsBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	PickUpObjectMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Mesh"));
	PickUpObjectMeshComponent->SetupAttachment(RootComponent);
}

void AamsuPickUpObjectsBase::BeginPlay()
{
	Super::BeginPlay();

	PickUpObjectMeshComponent->SetSimulatePhysics(true);
	PickUpObjectMeshComponent->SetMassOverrideInKg(NAME_None, ObjectMass, true);
	PickUpObjectMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	PickUpObjectMeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	PickUpObjectMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}


void AamsuPickUpObjectsBase::Interact_Implementation(AActor* InteractActor)
{
	// My TODO - this Interact supposed to let the character pick up the object and carry it (like the cubes in the Portal)
	APlayerController* PlayerController = Cast<APlayerController>(InteractActor);
	if (!IsValid(PlayerController))
	{
		return;
	}
	
}


