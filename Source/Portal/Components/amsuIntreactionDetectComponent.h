// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "amsuIntreactionDetectComponent.generated.h"

class IamsuInteractable;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PORTAL_API UamsuIntractionDetectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UamsuIntractionDetectComponent();

/** Returns nullptr if no object implementing IamsuInteractable is found within the specified distance */
	TScriptInterface<IamsuInteractable> GetAimedIfInteractable() const;  

private:
	UPROPERTY(EditAnywhere, Category = "Interact Detection Component")
	float CheckDistance = 200.f;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
