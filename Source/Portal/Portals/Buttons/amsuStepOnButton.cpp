// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuStepOnButton.h"

#include "Components/BoxComponent.h"


// Sets default values
AamsuStepOnButton::AamsuStepOnButton()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshComponentButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Base Mesh"));
	MeshComponentButtonBase->SetupAttachment(RootComponent);

	MeshComponentButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Mesh"));
	MeshComponentButton->SetupAttachment(RootComponent);

	BoxOverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Overlap Comnponent"));
	BoxOverlapComponent->SetupAttachment(RootComponent);
}

void AamsuStepOnButton::BeginPlay()
{
	Super::BeginPlay();
	
}

void AamsuStepOnButton::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetActorTickEnabled(true);

	ButtonPressed.Execute();
}

void AamsuStepOnButton::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SetActorTickEnabled(false);

	ButtonReleased.Execute();
}

void AamsuStepOnButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

