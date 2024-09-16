// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal/amsuInteractable.h"
#include "amsuDoorBase.generated.h"

UCLASS()
class PORTAL_API AamsuDoorBase : public AActor, public IamsuInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AamsuDoorBase();

	UFUNCTION(BlueprintCallable)
	virtual void Close();

	UFUNCTION(BlueprintCallable)
	virtual void Open();
	
	void Lock();
	void UnLock();
	void ToggleLock();

	virtual void Interact_Implementation(AActor* InteractActor) override;
	
	UPROPERTY(EditAnywhere, Category = "Door Parameters")
	/** Sets the initial door lock status */
	bool bIsLocked = false;
};
