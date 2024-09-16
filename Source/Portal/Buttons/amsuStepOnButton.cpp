// Fill out your copyright notice in the Description page of Project Settings.


#include "amsuStepOnButton.h"

#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(StepOnButon, Log, All);

// Sets default values
AamsuStepOnButton::AamsuStepOnButton()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	MeshComponentButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Base Mesh"));
	MeshComponentButtonBase->SetupAttachment(RootComponent);

	MeshComponentButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Mesh"));
	MeshComponentButton->SetupAttachment(RootComponent);

	BoxOverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Overlap Comnponent"));
	BoxOverlapComponent->SetupAttachment(RootComponent);
}

void AamsuStepOnButton::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	ButtonPress(DeltaSeconds);
	ButtonRelease(DeltaSeconds);
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
	if (!CanBeTriggeredBy(OtherActor))
	{
		return;
	}
	
	bButtonIsReleased = false;
	bButtonIsPressed = true;
	
	//ButtonPressed.Execute();
	ButtonPressed.Broadcast();
}

void AamsuStepOnButton::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!CanBeTriggeredBy(OtherActor))
	{
		return;
	}

	bButtonIsPressed = false;
	bButtonIsReleased = true;
	
	//TODO -My- Check if the Begin/End Overlap works with the needed actors (worked with character)
	//UE_LOG(StepOnButon, Log, TEXT("AamsuStepOnButton OnEndOverlap Called"));
	ButtonReleased.Broadcast();
}

bool AamsuStepOnButton::CanBeTriggeredBy(AActor* InOtherActor) const
{
	//UE_LOG(StepOnButon, Log, TEXT("AamsuStepOnButton OnBeginOverlap Called BEFORE FILTER"));
	if (!IsValid(InOtherActor))
	{
		return false;
	}

	if (TriggerActorClasses.IsEmpty())
	{
		return true;
	}
	
	for (const TSubclassOf<AActor>& Element : TriggerActorClasses)
	{
		if (InOtherActor->IsA(Element))
		{
			return true;
		}
	}

	return false;
}

void AamsuStepOnButton::ButtonPressRelease(float InDeltaTime, bool bPositivePress)
{
	float DeltaPress = InDeltaTime * PressSpeed;
	const float OldButtonPressBuffer = ButtonPressBuffer;
	float OldPlusDelta = 0.0f; 

	float MaxValue = 0.0f;
	float MinValue = 0.0f;
	
	if (bPositivePress)
	{
		OldPlusDelta = OldButtonPressBuffer - DeltaPress;
		//MaxValue = ButtonPressRange;
	}
	if (!bPositivePress)
	{
		OldPlusDelta = OldButtonPressBuffer + DeltaPress;
		//MinValue = -ButtonPressRange;
	}

	ButtonPressBuffer = FMath::Clamp<float>(OldPlusDelta, MinValue, MaxValue);

	DeltaPress = ButtonPressBuffer - OldButtonPressBuffer;

	const FVector DeltaVector(0.0f, 0.0f, DeltaPress);
	MeshComponentButton->AddRelativeLocation(DeltaVector);
}

void AamsuStepOnButton::ButtonPress(float InDeltaTime)
{
	if (bButtonIsPressed)
	{
		ButtonPressRelease(InDeltaTime, false);
	}
}

void AamsuStepOnButton::ButtonRelease(float InDeltaTime)
{
	if (bButtonIsReleased)
	{
		ButtonPressRelease(InDeltaTime, true);
	}
}


