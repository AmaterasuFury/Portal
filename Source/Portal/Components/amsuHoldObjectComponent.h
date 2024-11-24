// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "amsuHoldObjectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PORTAL_API UamsuHoldObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UamsuHoldObjectComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void PickUpAndCarry(AActor* InteractingActor);

	UPROPERTY(EditAnywhere, Category = "Hold Object Component")
	float HoldDistance = 150;

	UPROPERTY(EditAnywhere, Category = "Hold Object Component")
	float MovementSpeed = 10;
	 
protected:
	virtual void BeginPlay() override;
};
