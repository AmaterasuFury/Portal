// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuPickUpObjectsBase.h"
#include "Portal/Components/amsuHoldObjectComponent.h"

#include "Portal/MyCollisionChannels.h"

AamsuPickUpObjectsBase::AamsuPickUpObjectsBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	PickUpObjectMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Mesh"));
	PickUpObjectMeshComponent->SetupAttachment(RootComponent);

	PickUpObjectMeshComponent->SetCollisionProfileName(TEXT("Custom"));
	PickUpObjectMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickUpObjectMeshComponent->SetCollisionResponseToChannel(Portal::ECC_Interaction, ECR_Block);
	PickUpObjectMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
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
