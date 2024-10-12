// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal/amsuInteractable.h"
#include "AamsuPickUpObjectsBase.generated.h"

class UStaticMeshComponent;

UCLASS()
class PORTAL_API AamsuPickUpObjectsBase : public AActor, public IamsuInteractable
{
	GENERATED_BODY()

public:
	AamsuPickUpObjectsBase();
	
protected:
	virtual void BeginPlay() override;
	
public:

	UPROPERTY(EditAnywhere)
	float ObjectMass = 100.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pick Up Object")
	TObjectPtr<UStaticMeshComponent> PickUpObjectMeshComponent;
	
	virtual void Interact_Implementation(AActor* InteractActor) override;
};
