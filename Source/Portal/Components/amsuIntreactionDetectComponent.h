// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "amsuIntreactionDetectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PORTAL_API UamsuIntreactionDetectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UamsuIntreactionDetectComponent();

	UPROPERTY(EditAnywhere, Category = "Interaction Detection Component")
	float CheckDistance = 200.f;

/** Returns nullptr if no object implementing IamsuInteractable is found within the specified distance */
	AActor* DetectInteractable();  
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
