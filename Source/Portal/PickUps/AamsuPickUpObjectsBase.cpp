// Fill out your copyright notice in the Description page of Project Settings.


#include "AamsuPickUpObjectsBase.h"
#include "Portal/Components/amsuHoldObjectComponent.h"

#include "Portal/amsuMyCollisionChannels.h"

AamsuPickUpObjectsBase::AamsuPickUpObjectsBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	PickUpObjectMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Mesh"));
	PickUpObjectMeshComponent->SetupAttachment(RootComponent);

	PickUpObjectMeshComponent->SetCollisionProfileName(TEXT("Custom"));
	PickUpObjectMeshComponent->SetCollisionResponseToChannel(Portal::ECC_Interaction, ECR_Block);
	
	HoldObjectComponent = CreateDefaultSubobject<UamsuHoldObjectComponent>(TEXT("Hold Object Component"));
}



void AamsuPickUpObjectsBase::Interact_Implementation(AActor* InteractActor)
{
	if (!HoldObjectComponent->bAlreadyHolding)
	{
		HoldObjectComponent->PickUpStart(InteractActor);
	}
	else
	{
		HoldObjectComponent->PickUpEnd();
	}
}
