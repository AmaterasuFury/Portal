// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuDoorBase.h"


// Sets default values
AamsuDoorBase::AamsuDoorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

/** A base class CloseDoor function with empty implementation */
void AamsuDoorBase::Close() {}

/** A base class OpenDoor function with empty implementation */
void AamsuDoorBase::Open() {}

void AamsuDoorBase::Lock()
{
	bIsLocked = true;
}

void AamsuDoorBase::UnLock()
{
	bIsLocked = false;
}

void AamsuDoorBase::ToggleLock()
{
	bIsLocked ^= 1;
}

void AamsuDoorBase::Interact_Implementation(AActor* InteractActor)
{
	ToggleLock();
}



