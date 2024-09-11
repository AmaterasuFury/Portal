// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuStepOnButton.h"

#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(StepOnButon, Log, All);

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

	BoxOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AamsuStepOnButton::OnBeginOverlap);
	BoxOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &AamsuStepOnButton::OnEndOverlap);
}

void AamsuStepOnButton::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FilterOverlapped(OtherActor);
	
	//ButtonPressed.Execute();
}

void AamsuStepOnButton::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FilterOverlapped(OtherActor);
	
	//TODO -My- Check if the Begin/End Overlap works with the needed actors (worked with character)
	//UE_LOG(StepOnButon, Log, TEXT("AamsuStepOnButton OnEndOverlap Called"));
	//ButtonReleased.Execute();
}

void AamsuStepOnButton::FilterOverlapped(AActor* InOtherActor)
{
	//UE_LOG(StepOnButon, Log, TEXT("AamsuStepOnButton OnBeginOverlap Called BEFORE FILTER"));
	if (!IsValid(InOtherActor))
	{
		return;
	}
	for (const TSubclassOf<AActor> Element : TriggerActorClasses)
	{
		if (InOtherActor->IsA(Element))
		{
			//ButtonPressed.Execute();
			UE_LOG(StepOnButon, Log, TEXT("AamsuStepOnButton OnBeginOverlap/OnEndOverlap Filtered and called"));
		}
	}
}


