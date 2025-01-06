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

	UPROPERTY()
	TObjectPtr<AActor> InteractingActor;
	
	void PickUpStart(AActor* InInteractedActor);
	void PickUpEnd();
	
	void PickUpAndCarry(float InDeltaTime);

	UPROPERTY(EditAnywhere, Category = "Hold Object Component")
	float HoldDistance = 150.f;

	UPROPERTY(EditAnywhere, Category = "Hold Object Component")
	float MovementSpeed = 1000.f;

	/** Time before dropping an object if it is not in front of a character or something is between them*/
	UPROPERTY(EditAnywhere, Category = "Hold Object Component")
	float ObjectReleaseTime = 1.f;
	 
protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle TimerTillDrop;
};
