// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bridge.generated.h"

class UArrowComponent;

UCLASS()
class PORTAL_API ABridge : public AActor
{
	GENERATED_BODY()

public:
	ABridge();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bridge Generator")
	TObjectPtr<UStaticMeshComponent> MeshComponentBridgeGenerator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "First BridgePart")
	TObjectPtr<UStaticMeshComponent> MeshFirstBridgePart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> InitialBridgeTransform;

	UPROPERTY(EditAnywhere)
	float MaxBridgeLength = 1000.f;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SpawnBridge();

	UFUNCTION(BlueprintCallable)
	void DestroyBridge();

	UPROPERTY(EditDefaultsOnly)
	double BridgePartLength = 100.f;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> BridgeParts;

	// todo make a linetrace to get distance and use the distance to spawn the bridge
};
