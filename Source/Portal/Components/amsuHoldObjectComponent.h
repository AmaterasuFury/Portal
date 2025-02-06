// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "amsuHoldObjectComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHoldObjectComponent, Log, All);

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

//  ToDO Refactor or delete 	
//	/** The distance tha will not be exceeded when the character holds an object and rotates 
//	 * (So the object would not "go through the character */
//	UPROPERTY(EditAnywhere, Category = "Hold Object Component")
//	float MinMoveDistance = 50.f;

	UPROPERTY(EditAnywhere, Category = "Hold Object Component")
	float MovementSpeed = 1500.f;

	/** Time before dropping an object if it is not in front of a character or something is between them*/
	UPROPERTY(EditAnywhere, Category = "Hold Object Component")
	float ObjectReleaseTime = 0.2f;

	bool IsAlreadyHolding() const;
	 
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	FTimerHandle TimerTillDrop;

	bool bAlreadyHolding = false;
};
