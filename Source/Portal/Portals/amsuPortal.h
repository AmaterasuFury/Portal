// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "amsuPortal.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

DECLARE_MULTICAST_DELEGATE(FPortalStateDelegate)

UCLASS()
class PORTAL_API AamsuPortal : public AActor
{
	GENERATED_BODY()

public:
	AamsuPortal();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<UStaticMeshComponent> MeshComponentPortal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<UBoxComponent> BoxOverlapComponent;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY()
	TObjectPtr<AamsuPortal> AnotherPortal; 
	
	// TODO use not just this boll, but an ActivatePortal() function to: hide the actor, turn tick off (if needed), turn physic off, etc.
	bool bIsActive = false;

	UFUNCTION()
	void ActivatePortal(bool bActivate);

	FPortalStateDelegate OnPortalStateChange;

	void Teleport(AActor* InteractedActor);

	void SetPortalVisibility(bool bMakeVisible);
};