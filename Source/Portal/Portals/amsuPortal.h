// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "amsuPortal.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(amsuPortal, Log, Warning)

DECLARE_MULTICAST_DELEGATE_OneParam(FPortalStateDelegate, bool /* IsActive */)

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
	TObjectPtr<UStaticMeshComponent> MeshComponentPortalFrame;

	//* This mesh is supposed to become invisible on the BeginPlay, and colidible with a character only, but only
	// in case of collisions with the portal*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<UStaticMeshComponent> InvisibleBackFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<UBoxComponent> BoxOverlapComponent;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY(Transient)
	TObjectPtr<AamsuPortal> AnotherPortal;

	UPROPERTY(EditAnywhere, Category = "Portal")
	TObjectPtr<UMaterialInterface> ActivePortalMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Portal")
	TObjectPtr<UMaterialInterface> InactivePortalMaterial;
	
	bool IsPortalVisible() const;
	
	FPortalStateDelegate OnPortalStateChange;
	
	void TeleportStart(AActor* InteractedActor) const;
	void TeleportEnd(AActor* InteractingActor) const;

	void IgnoreOverlappedActor(AActor* ColidedACtor, bool bIgnore) const;

	void MakePortalVisible(bool bMakeVisible);

	/** Check what actors the portal is placed on and set it to the array*/
	void AssignPortalAttachedActors(const FVector& PortalSpawnLocation, const FRotator& PortalSpawnRotation);
	
	float PortalsHalfWidth = 0.0f;
	float PortalsHalfHeight = 0.0f;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> PortalAttachedActors;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneCaptureComponentCube> CaptureComponentCube;

	bool bIsVisible = true;
};