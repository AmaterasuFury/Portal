// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "amsuPortal.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

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
	TObjectPtr<UStaticMeshComponent> MeshComponentInactivePortal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<UStaticMeshComponent> MeshComponentActivePortal;

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
	
	bool IsPortalVisible() const;
	
	FPortalStateDelegate OnPortalStateChange;

	void Teleport(AActor* InteractedActor) const;

	void MakePortalVisible(bool bMakeVisible);
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneCaptureComponentCube> CaptureComponentCube;

	bool bIsVisible = true;
};