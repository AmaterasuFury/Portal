// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "amsuIntreactionDetectComponent.generated.h"

class IamsuInteractable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PORTAL_API UamsuInteractionDetectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UamsuInteractionDetectComponent();

/** Returns nullptr if no object implementing IamsuInteractable is found within the specified distance */
	TScriptInterface<IamsuInteractable> GetAimedInteractable() const;  

private:
	UPROPERTY(EditAnywhere, Category = "Interact Detection Component")
	float CheckDistance = 200.f;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
