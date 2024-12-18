// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuPickUpObjectsBase.h"
#include "Portal/Components/amsuHoldObjectComponent.h"


AamsuPickUpObjectsBase::AamsuPickUpObjectsBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	PickUpObjectMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Mesh"));
	PickUpObjectMeshComponent->SetupAttachment(RootComponent);

	HoldObjectComponent = CreateDefaultSubobject<UamsuHoldObjectComponent>(TEXT("Hold Object Component"));
}

void AamsuPickUpObjectsBase::BeginPlay()
{
	Super::BeginPlay();

	//PickUpObjectMeshComponent->SetSimulatePhysics(true);
	//PickUpObjectMeshComponent->SetMassOverrideInKg(NAME_None, ObjectMass, true);
	//PickUpObjectMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//PickUpObjectMeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	//PickUpObjectMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}


void AamsuPickUpObjectsBase::Interact_Implementation(AActor* InteractActor)
{
	HoldObjectComponent->PickUpStart(InteractActor);
}


