// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "amsuStepOnButton.generated.h"

class UBoxComponent;

DECLARE_DELEGATE(FButtonPressedEvent)
DECLARE_DELEGATE(FButtonReleasedEvent)

UCLASS()
class PORTAL_API AamsuStepOnButton : public AActor
{
	GENERATED_BODY()

public:
	AamsuStepOnButton();
	
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button")
	TObjectPtr<UStaticMeshComponent> MeshComponentButtonBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button")
	TObjectPtr<UStaticMeshComponent> MeshComponentButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button")
	TObjectPtr<UBoxComponent> BoxOverlapComponent;

	FButtonPressedEvent ButtonPressed;
	FButtonReleasedEvent ButtonReleased;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// TODO
	//UPROPERTY(EditDefaultsOnly)
	//TArray<TSubclassOf<AActor>> TriggerActorClasses;
};
