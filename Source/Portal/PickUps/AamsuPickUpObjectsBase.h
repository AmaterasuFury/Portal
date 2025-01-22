// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal/amsuInteractable.h"
#include "AamsuPickUpObjectsBase.generated.h"

class UamsuHoldObjectComponent;
class UStaticMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(amsuPickUpObjectsBase, Log, All);

UCLASS()
class PORTAL_API AamsuPickUpObjectsBase : public AActor, public IamsuInteractable
{
	GENERATED_BODY()

public:
	AamsuPickUpObjectsBase();

	UPROPERTY(EditAnywhere)
	float ObjectMass = 100.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pick Up Object")
	TObjectPtr<UStaticMeshComponent> PickUpObjectMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pick Up Object")
	TObjectPtr<UamsuHoldObjectComponent> HoldObjectComponent;
	
	virtual void Interact_Implementation(AActor* InteractActor) override;

	void PhysicsToggle(bool bPhysicsOn) const;
};
